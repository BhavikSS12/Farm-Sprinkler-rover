#include "platform/rover_config.h"
#include "board/board.h"
#include "drivers/encoder.h"
#include "drivers/motor_controller.h"
#include "drivers/pump_controller.h"
#include "drivers/serial_cmd.h"

static void blink_ready(void)
{
    for (int i = 0; i < 3; i++) {
        led_on();
        delay_ms(200);
        led_off();
        delay_ms(200);
    }
}

int main(void)
{
    board_init();
    motorcontrol_init();
    pumpcontrol_init();
    serialcmd_init();
    encoder_init();

    driveStop();
    disableDrivers();
    pumpOff();

    blink_ready();
    uart1_puts("STM32_READY\r\n");

    while (1) {
        serialCmd_poll();
    }
}