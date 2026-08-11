#include "bsp_pwm.h"
#include "stm32f10x.h"
#include "bsp_gpio.h"

static void timerBaseInit(TIM_TypeDef *tim){
	tim->PSC = 8;
	tim->ARR = 255;
	tim->CR1 |= TIM_CR1_ARPE;
	tim->EGR = TIM_EGR_UG;
}

void pwmInit(void){
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM4EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
	
	AFIO->MAPR = (AFIO->MAPR & ~(0x03U << 10)) | (0x02U << 10);
	
	AFIO->MAPR = (AFIO->MAPR & ~(0x03U << 8)) | (0x01U << 8);
	
	gpioInit(GPIOB , 3, GPIO_AF_PP_50);
	gpioInit(GPIOB , 4, GPIO_AF_PP_50);
	gpioInit(GPIOB , 5, GPIO_AF_PP_50);
	gpioInit(GPIOB , 6, GPIO_AF_PP_50);
	gpioInit(GPIOB , 7, GPIO_AF_PP_50);
	
    /* TIM3: RPWM_A (CH1=PB4), LPWM_A (CH2=PB5) */
    timerBaseInit(TIM3);
    TIM3->CCMR1 = (6U << 4) | (1U << 3) | (6U << 12) | (1U << 11); /* PWM1, preload */
    TIM3->CCR1  = 0; TIM3->CCR2 = 0;
    TIM3->CCER  = TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM3->CR1  |= TIM_CR1_CEN;

    /* TIM4: RPWM_B (CH1=PB6), LPWM_B (CH2=PB7) */
    timerBaseInit(TIM4);
    TIM4->CCMR1 = (6U << 4) | (1U << 3) | (6U << 12) | (1U << 11);
    TIM4->CCR1  = 0; TIM4->CCR2 = 0;
    TIM4->CCER  = TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM4->CR1  |= TIM_CR1_CEN;

    /* TIM2: PUMP_ENA (CH2=PB3) */
    timerBaseInit(TIM2);
    TIM2->CCMR1 = (6U << 12) | (1U << 11);   /* CH2 PWM1 + preload */
    TIM2->CCR2  = 0;
    TIM2->CCER  = TIM_CCER_CC2E;
    TIM2->CR1  |= TIM_CR1_CEN;
}

void pwmSet(PWM_channel_t ch, uint8_t duty){
	switch(ch){
		case RPWM_A: TIM3->CCR1 = duty; break;
		case LPWM_A: TIM3->CCR2 = duty; break;
		case RPWM_B: TIM4->CCR1 = duty; break;
		case LPWM_B: TIM4->CCR2 = duty; break;
		case PWM_pump_ena: TIM2->CCR2 = duty; break;
		default: break;
	}
}