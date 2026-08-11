#include "command_parser.h"
#include "BSP/bsp_uart.h"
#include "Drivers/motor.h"
#include "Drivers/pump.h"
#include "Drivers/encoder.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/* Simple string-to-int that handles the "X:<value>" format */
static int parse_val(const char *s, uint8_t offset) {
    int v = atoi(s + offset);
    if (v < 0)   v = 0;
    if (v > 255) v = 255;
    return v;
}

static int str_starts(const char *s, const char *prefix) {
    return strncmp(s, prefix, strlen(prefix)) == 0;
}

void CMD_Parse(const char *line) {

    /* F:<pwm>  → Forward */
    if (str_starts(line, "F:")) {
        Motor_Forward((uint8_t)parse_val(line, 2));
    }
    /* R:<pwm>  → Reverse */
    else if (str_starts(line, "R:")) {
        Motor_Reverse((uint8_t)parse_val(line, 2));
    }
    /* L:<pwm>  → Turn Left (pivot) */
    else if (str_starts(line, "L:")) {
        Motor_TurnLeft((uint8_t)parse_val(line, 2));
    }
    /* T:<pwm>  → Turn Right (pivot) */
    else if (str_starts(line, "T:")) {
        Motor_TurnRight((uint8_t)parse_val(line, 2));
    }
    /* S  → Stop all drive motors */
    else if (strcmp(line, "S") == 0) {
        Motor_Stop();
    }
    /* E  → Report encoder counts */
    else if (strcmp(line, "E") == 0) {
        BSP_UART_SendStr("A:");
        BSP_UART_SendInt(Encoder_ReadA());
        BSP_UART_SendStr(",B:");
        BSP_UART_SendInt(Encoder_ReadB());
        BSP_UART_SendChar('\n');
    }
    /* RESET  → Zero encoder counts */
    else if (strcmp(line, "RESET") == 0) {
        Encoder_Reset();
        BSP_UART_SendStr("OK\n");
    }
    /* PUMP_ON:<pwm>  → Start pump */
    else if (str_starts(line, "PUMP_ON:")) {
        Pump_On((uint8_t)parse_val(line, 8));
        BSP_UART_SendStr("PUMP_ON\n");
    }
    /* PUMP_OFF  → Stop pump */
    else if (strcmp(line, "PUMP_OFF") == 0) {
        Pump_Off();
        BSP_UART_SendStr("PUMP_OFF\n");
    }
    /* Unknown → silent ignore (or uncomment for debug) */
    /* else { BSP_UART_SendStr("ERR\n"); } */
}