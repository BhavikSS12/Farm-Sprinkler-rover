#include "pump.h"
#include "BSP/bsp_gpio.h"
#include "BSP/bsp_pwm.h"
#include "stm32f10x.h"

#define PUMP_IN1_PIN  2   /* PA2 */
#define PUMP_IN2_PIN  3   /* PA3 */

void Pump_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    BSP_GPIO_Init(GPIOA, PUMP_IN1_PIN, GPIO_OUT_PP_50);
    BSP_GPIO_Init(GPIOA, PUMP_IN2_PIN, GPIO_OUT_PP_50);
    BSP_GPIO_Reset(GPIOA, PUMP_IN1_PIN);
    BSP_GPIO_Reset(GPIOA, PUMP_IN2_PIN);
    /* PB3 / TIM2 CH2 is already inited by BSP_PWM_Init() in Motor_Init() */
    BSP_PWM_Set(PWM_PUMP_ENA, 0);
}

void Pump_On(uint8_t speed) {
    BSP_GPIO_Set(GPIOA, PUMP_IN1_PIN);
    BSP_GPIO_Reset(GPIOA, PUMP_IN2_PIN);
    BSP_PWM_Set(PWM_PUMP_ENA, speed);
}

void Pump_Off(void) {
    BSP_PWM_Set(PWM_PUMP_ENA, 0);
    BSP_GPIO_Reset(GPIOA, PUMP_IN1_PIN);
}