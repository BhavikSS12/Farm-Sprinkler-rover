#include "../platform/rover_config.h"
#include "../drivers/encoder.h"

void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & (1U << ENCODER_A_BIT)) {
        encoderISR_A();
        EXTI->PR = (1U << ENCODER_A_BIT);
    }
}

void EXTI1_IRQHandler(void)
{
    if (EXTI->PR & (1U << ENCODER_B_BIT)) {
        encoderISR_B();
        EXTI->PR = (1U << ENCODER_B_BIT);
    }
}