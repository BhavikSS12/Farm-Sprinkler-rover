#include "stm32f10x.h"
#include "BSP/bsp_systick.h"
#include "BSP/bsp_gpio.h"
#include "BSP/bsp_uart.h"
#include "Drivers/motor.h"
#include "Drivers/pump.h"
#include "Drivers/encoder.h"
#include "App/command_parser.h"

/* ── ISRs ────────────────────────────────────────────────── */
#define LED_PORT  GPIOB
#define LED_PIN   2

void SysTick_Handler(void) {
    sysTick_IRQHandler();
}

void EXTI0_IRQHandler(void) {
    Encoder_ISR_A();
}

void EXTI1_IRQHandler(void) {
    Encoder_ISR_B();
}

/* ── Helpers ─────────────────────────────────────────────── */

static void LED_Blink(uint8_t times) {
    for (uint8_t i = 0; i < times; i++) {
        gpioReset(LED_PORT, LED_PIN);  /* ON  (active LOW) */
        delayMs(200);
        gpioSet(LED_PORT, LED_PIN);    /* OFF */
        delayMs(200);
    }
}

/* ── Main ────────────────────────────────────────────────── */

int main(void) {
    /* 1. SysTick first — other modules use getMicros() */
    sysTick_Init();

    /* 2. Enable GPIOB clock and configure LED */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    gpioInit(LED_PORT, LED_PIN, GPIO_OUT_PP_50);
    gpioSet(LED_PORT, LED_PIN);   /* LED off */

    /* 3. Peripherals */
    Motor_Init();       /* also calls pwmInit() */
    Pump_Init();
    Encoder_Init();
    uartInit(115200);

    delayMs(1000);    /* settling time */

    /* 4. Blink 3× = ready */
    LED_Blink(3);

    uartSendStr("STM32_READY\n");

    /* 5. Main loop */
    char rxbuf[64];
    while (1) {
        if (uartReadLine(rxbuf, sizeof(rxbuf))) {
            CMD_Parse(rxbuf);
        }
    }
}