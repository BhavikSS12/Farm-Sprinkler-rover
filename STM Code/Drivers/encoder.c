#include "encoder.h"
#include "../BSP/bsp_gpio.h"
#include "../BSP/bsp_systick.h"
#include "stm32f10x.h"   /* ← changed */

#define DEBOUNCE_US 1500U

static volatile int32_t  pulseA = 0, pulseB = 0;
static volatile uint32_t lastA  = 0, lastB  = 0;

void Encoder_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
    BSP_GPIO_Init(GPIOA, 0, GPIO_IN_PUPD);
    BSP_GPIO_Init(GPIOA, 1, GPIO_IN_PUPD);
    GPIOA->ODR |= (1U << 0) | (1U << 1);   /* pull-up */

    EXTI->IMR  |= EXTI_IMR_MR0  | EXTI_IMR_MR1;
    EXTI->RTSR |= EXTI_RTSR_TR0 | EXTI_RTSR_TR1;

    NVIC_SetPriority(EXTI0_IRQn, 1);
    NVIC_SetPriority(EXTI1_IRQn, 1);
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI1_IRQn);
}

void Encoder_ISR_A(void) {
    uint32_t now = BSP_GetMicros();
    if ((now - lastA) > DEBOUNCE_US) { pulseA++; lastA = now; }
    EXTI->PR = EXTI_PR_PR0;
}

void Encoder_ISR_B(void) {
    uint32_t now = BSP_GetMicros();
    if ((now - lastB) > DEBOUNCE_US) { pulseB++; lastB = now; }
    EXTI->PR = EXTI_PR_PR1;
}

int32_t Encoder_ReadA(void) {
    int32_t v;
    __disable_irq(); v = pulseA; __enable_irq();   /* ← CMSIS intrinsics */
    return v;
}

int32_t Encoder_ReadB(void) {
    int32_t v;
    __disable_irq(); v = pulseB; __enable_irq();
    return v;
}

void Encoder_Reset(void) {
    __disable_irq();
    pulseA = pulseB = 0; lastA = lastB = 0;
    __enable_irq();
}