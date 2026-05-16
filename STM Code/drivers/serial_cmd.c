#include "../platform/rover_config.h"
#include "../board/board.h"
#include "encoder.h"
#include "motor_controller.h"
#include "pump_controller.h"
#include "serial_cmd.h"

#define CMD_BUF_LEN 32U

static char rx_buf[CMD_BUF_LEN];
static uint8_t rx_idx = 0;

static uint8_t str_eq(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return (*a == '\0' && *b == '\0');
}

static uint8_t str_starts(const char *s, const char *p)
{
    while (*p) {
        if (*s++ != *p++) return 0;
    }
    return 1;
}

static uint8_t parse_u8(const char *s)
{
    uint16_t v = 0;
    while (*s >= '0' && *s <= '9') {
        v = (uint16_t)(v * 10u + (uint16_t)(*s - '0'));
        s++;
    }
    if (v > 255u) v = 255u;
    return (uint8_t)v;
}

static void uart_print_i32(int32_t v)
{
    char buf[12];
    uint8_t i = 0, j;

    if (v == 0) {
        uart1_putc('0');
        return;
    }

    if (v < 0) {
        uart1_putc('-');
        v = -v;
    }

    while (v > 0 && i < 11u) {
        buf[i++] = (char)('0' + (v % 10));
        v /= 10;
    }

    for (j = i; j > 0u; j--) {
        uart1_putc(buf[j - 1u]);
    }
}

void serialcmd_init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    USART1->BRR  = USART1_BRR_VALUE;
    USART1->CR1  = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

static void dispatch(char *cmd)
{
    uint8_t len = 0;
    while (cmd[len]) len++;
    if (len > 0u && cmd[len - 1u] == '\r') cmd[--len] = '\0';
    if (len == 0u) return;

    if (str_starts(cmd, "F:")) {
        uint8_t spd = parse_u8(cmd + 2);
        enableDrivers();
        driveForward(spd);

    } else if (str_starts(cmd, "R:")) {
        uint8_t spd = parse_u8(cmd + 2);
        enableDrivers();
        driveReverse(spd);

    } else if (str_starts(cmd, "L:")) {
        uint8_t spd = parse_u8(cmd + 2);
        enableDrivers();
        turnLeft(spd);

    } else if (str_starts(cmd, "T:")) {
        uint8_t spd = parse_u8(cmd + 2);
        enableDrivers();
        turnRight(spd);

    } else if (str_eq(cmd, "S")) {
        driveStop();
        disableDrivers();

    } else if (str_eq(cmd, "E")) {
        uart1_puts("A:");
        uart_print_i32(readPulsesA());
        uart1_puts(",B:");
        uart_print_i32(readPulsesB());
        uart1_puts("\r\n");

    } else if (str_eq(cmd, "RESET")) {
        resetEncoders();
        uart1_puts("OK\r\n");

    } else if (str_starts(cmd, "PUMP_ON:")) {
        uint8_t spd = parse_u8(cmd + 8);
        pumpOn(spd);
        uart1_puts("PUMP_ON\r\n");

    } else if (str_eq(cmd, "PUMP_OFF")) {
        pumpOff();
        uart1_puts("PUMP_OFF\r\n");
    }
}

void serialCmd_rxByte(uint8_t byte)
{
    if (byte == '\n') {
        rx_buf[rx_idx] = '\0';
        dispatch(rx_buf);
        rx_idx = 0;
    } else {
        if (rx_idx < CMD_BUF_LEN - 1u) {
            rx_buf[rx_idx++] = (char)byte;
        } else {
            rx_idx = 0;
        }
    }
}

void serialCmd_poll(void)
{
    while (USART1->SR & USART_SR_RXNE) {
        serialCmd_rxByte((uint8_t)USART1->DR);
    }
}