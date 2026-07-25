#include "bsp_pwn.h"
#include "stm32f10x.h"
#include "bsp_gpio.h"

static void timerBaseInit(TIM_TypeDef *tim){
	tim->PSC = 8 ;
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
	
	timerBaseInit(TIM3);
	TIM3->CCMR1
}