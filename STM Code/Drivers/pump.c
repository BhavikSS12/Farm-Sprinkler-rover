#include "pump.h"
#include "BSP/bsp_gpio.h"
#include "BSP/bsp_pwm.h"
#include "stm32f10x.h"

#define PUMP_IN1_PIN  2   /* PA2 */
#define PUMP_IN2_PIN  3   /* PA3 */

void Pump_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    gpioInit(GPIOA, PUMP_IN1_PIN, GPIO_OUT_PP_50);
    gpioInit(GPIOA, PUMP_IN2_PIN, GPIO_OUT_PP_50);
    gpioReset(GPIOA, PUMP_IN1_PIN);
    gpioReset(GPIOA, PUMP_IN2_PIN);
    /* PB3 / TIM2 CH2 is already inited by pwmInit() in Motor_Init() */
    pwmSet(PWM_pump_ena, 0);
}

void Pump_On(uint8_t speed) {
    gpioSet(GPIOA, PUMP_IN1_PIN);
    gpioReset(GPIOA, PUMP_IN2_PIN);
    pwmSet(PWM_pump_ena, speed);
}

void Pump_Off(void) {
    pwmSet(PWM_pump_ena, 0);
    gpioReset(GPIOA, PUMP_IN1_PIN);
}