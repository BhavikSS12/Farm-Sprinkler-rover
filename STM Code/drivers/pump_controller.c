#include "../platform/rover_config.h"
#include "pump_controller.h"

void pumpcontrol_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->PSC   = PWM_PSC;
    TIM2->ARR   = PWM_ARR;
    TIM2->CCR2  = 0;
    TIM2->CCMR1 = TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE;
    TIM2->CCER  = TIM_CCER_CC2E;
    TIM2->CR1   = TIM_CR1_ARPE | TIM_CR1_CEN;
    TIM2->EGR   = TIM_EGR_UG;

    GPIOA->BRR = (1U << PUMP_IN1_BIT) | (1U << PUMP_IN2_BIT);
}

void pumpOn(uint8_t speed)
{
    GPIOA->BSRR = (1U << PUMP_IN1_BIT);
    GPIOA->BRR  = (1U << PUMP_IN2_BIT);
    TIM2->CCR2  = speed;
}

void pumpOff(void)
{
    TIM2->CCR2 = 0;
    GPIOA->BRR = (1U << PUMP_IN1_BIT);
}