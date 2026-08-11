#include "motor.h"
#include "BSP/bsp_gpio.h"
#include "BSP/bsp_pwm.h"
#include "stm32f10x.h"

/* Enable pins */
#define R_EN_A_PIN  0    /* PB0 */
#define L_EN_A_PIN  1    /* PB1 */
#define R_EN_B_PIN  8    /* PB8 */
#define L_EN_B_PIN  9    /* PB9 */

void Motor_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    BSP_GPIO_Init(GPIOB, R_EN_A_PIN, GPIO_OUT_PP_50);
    BSP_GPIO_Init(GPIOB, L_EN_A_PIN, GPIO_OUT_PP_50);
    BSP_GPIO_Init(GPIOB, R_EN_B_PIN, GPIO_OUT_PP_50);
    BSP_GPIO_Init(GPIOB, L_EN_B_PIN, GPIO_OUT_PP_50);
    Motor_DisableAll();
    BSP_PWM_Init();
}

void Motor_EnableAll(void) {
    BSP_GPIO_Set(GPIOB, R_EN_A_PIN); BSP_GPIO_Set(GPIOB, L_EN_A_PIN);
    BSP_GPIO_Set(GPIOB, R_EN_B_PIN); BSP_GPIO_Set(GPIOB, L_EN_B_PIN);
}

void Motor_DisableAll(void) {
    BSP_GPIO_Reset(GPIOB, R_EN_A_PIN); BSP_GPIO_Reset(GPIOB, L_EN_A_PIN);
    BSP_GPIO_Reset(GPIOB, R_EN_B_PIN); BSP_GPIO_Reset(GPIOB, L_EN_B_PIN);
}

void Motor_A_Forward(uint8_t s) { BSP_PWM_Set(PWM_LPWM_A, s); BSP_PWM_Set(PWM_RPWM_A, 0); }
void Motor_A_Reverse(uint8_t s) { BSP_PWM_Set(PWM_LPWM_A, 0); BSP_PWM_Set(PWM_RPWM_A, s); }
void Motor_A_Stop(void)         { BSP_PWM_Set(PWM_LPWM_A, 0); BSP_PWM_Set(PWM_RPWM_A, 0); }

void Motor_B_Forward(uint8_t s) { BSP_PWM_Set(PWM_RPWM_B, s); BSP_PWM_Set(PWM_LPWM_B, 0); }
void Motor_B_Reverse(uint8_t s) { BSP_PWM_Set(PWM_RPWM_B, 0); BSP_PWM_Set(PWM_LPWM_B, s); }
void Motor_B_Stop(void)         { BSP_PWM_Set(PWM_RPWM_B, 0); BSP_PWM_Set(PWM_LPWM_B, 0); }

void Motor_Forward(uint8_t s)  { Motor_EnableAll(); Motor_A_Forward(s); Motor_B_Forward(s); }
void Motor_Reverse(uint8_t s)  { Motor_EnableAll(); Motor_A_Reverse(s); Motor_B_Reverse(s); }
void Motor_TurnLeft(uint8_t s) { Motor_EnableAll(); Motor_A_Reverse(s); Motor_B_Forward(s); }
void Motor_TurnRight(uint8_t s){ Motor_EnableAll(); Motor_A_Forward(s); Motor_B_Reverse(s); }
void Motor_Stop(void)          { Motor_A_Stop(); Motor_B_Stop(); Motor_DisableAll(); }