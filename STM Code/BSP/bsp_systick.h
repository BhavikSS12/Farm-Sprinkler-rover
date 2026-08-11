#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f10x.h"
#include <stdint.h>

void sysTick_Init(void);
void sysTick_IRQHandler(void);  /* call from SysTick_Handler */
uint32_t getTick(void);             /* milliseconds */
uint32_t getMicros(void);           /* microseconds (coarse) */
void delayMs(uint32_t ms);
uint8_t isTimeout(uint32_t startTick , uint32_t timeoutMs);

#endif