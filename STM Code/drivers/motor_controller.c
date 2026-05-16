#include "C:\Users\BHAVIK\Desktop\Bhavik_clg\Projects\Embedded_projects\Farm_sprinkler_rover\Farm-Sprinkler-rover\STM Code\platform\rover_config.h"
#include "motor_controller.h"

static void tim_init_pwm_ch1_ch2(TIM_TypeDef *tim)
{
    tim->PSC   = PWM_PSC;
    tim->ARR   = PWM_ARR;
    tim->CCR1  = 0;
    tim->CCR2  = 0;
    tim->CCMR1 = TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE |
                 TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;
    tim->CCER  = TIM_CCER_CC1E | TIM_CCER_CC2E;
    tim->CR1   = TIM_CR1_ARPE | TIM_CR1_CEN;
    tim->EGR   = TIM_EGR_UG;
}

void motorcontrol_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM4EN;
    tim_init_pwm_ch1_ch2(TIM3);
    tim_init_pwm_ch1_ch2(TIM4);
}

void driverA_Forward(uint8_t speed) { TIM3->CCR2 = speed; TIM3->CCR1 = 0; }
void driverA_Reverse(uint8_t speed) { TIM3->CCR2 = 0; TIM3->CCR1 = speed; }
void driverA_Stop(void)             { TIM3->CCR2 = 0; TIM3->CCR1 = 0; }

void driverB_Forward(uint8_t speed) { TIM4->CCR1 = speed; TIM4->CCR2 = 0; }
void driverB_Reverse(uint8_t speed) { TIM4->CCR1 = 0; TIM4->CCR2 = speed; }
void driverB_Stop(void)             { TIM4->CCR1 = 0; TIM4->CCR2 = 0; }

void driveForward(uint8_t speed) { driverA_Forward(speed); driverB_Forward(speed); }
void driveReverse(uint8_t speed) { driverA_Reverse(speed); driverB_Reverse(speed); }
void driveStop(void)             { driverA_Stop(); driverB_Stop(); }
void turnLeft(uint8_t speed)     { driverA_Reverse(speed); driverB_Forward(speed); }
void turnRight(uint8_t speed)    { driverA_Forward(speed); driverB_Reverse(speed); }

void enableDrivers(void)
{
    GPIOB->BSRR = (1U << R_EN_A_BIT) | (1U << L_EN_A_BIT) |
                  (1U << R_EN_B_BIT) | (1U << L_EN_B_BIT);
}

void disableDrivers(void)
{
    GPIOB->BRR  = (1U << R_EN_A_BIT) | (1U << L_EN_A_BIT) |
                  (1U << R_EN_B_BIT) | (1U << L_EN_B_BIT);
}