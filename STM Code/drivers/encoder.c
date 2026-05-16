#include "../platform/rover_config.h"
#include "../board/board.h"
#include "encoder.h"

static volatile int32_t pulseCountA = 0;
static volatile int32_t pulseCountB = 0;
static volatile uint32_t lastTimeA = 0;
static volatile uint32_t lastTimeB = 0;

static inline uint32_t irq_disable(void)
{
    uint32_t primask;
    __asm volatile ("MRS %0, PRIMASK\n\t" "CPSID i" : "=r" (primask) :: "memory");
    return primask;
}

static inline void irq_restore(uint32_t primask)
{
    __asm volatile ("MSR PRIMASK, %0" :: "r" (primask) : "memory");
}

void encoder_init(void)
{
    AFIO->EXTICR[0] &= ~(0xFFU);
    EXTI->RTSR |= (1U << ENCODER_A_BIT) | (1U << ENCODER_B_BIT);
    EXTI->FTSR &= ~((1U << ENCODER_A_BIT) | (1U << ENCODER_B_BIT));
    EXTI->IMR  |= (1U << ENCODER_A_BIT) | (1U << ENCODER_B_BIT);
    NVIC_ISER0 |= (1U << 6) | (1U << 7);
}

void encoderISR_A(void)
{
    uint32_t now = micros();
    if ((now - lastTimeA) > DEBOUNCE_US) {
        pulseCountA++;
        lastTimeA = now;
    }
}

void encoderISR_B(void)
{
    uint32_t now = micros();
    if ((now - lastTimeB) > DEBOUNCE_US) {
        pulseCountB++;
        lastTimeB = now;
    }
}

int32_t readPulsesA(void)
{
    uint32_t key = irq_disable();
    int32_t v = pulseCountA;
    irq_restore(key);
    return v;
}

int32_t readPulsesB(void)
{
    uint32_t key = irq_disable();
    int32_t v = pulseCountB;
    irq_restore(key);
    return v;
}

void resetEncoders(void)
{
    uint32_t key = irq_disable();
    pulseCountA = 0;
    pulseCountB = 0;
    lastTimeA = 0;
    lastTimeB = 0;
    irq_restore(key);
}