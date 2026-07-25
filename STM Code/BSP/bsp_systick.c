#include "bsp_systick.h"

static volatile uint32_t tick = 0;

void sysTick_Init(void){
	SysTick-> LOAD = (SystemCoreClock / 1000U) - 1U;
	
	SysTick->VAL = 0U;
	
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk ;
}

void sysTick_IRQHandler(void){
	tick++ ;
}

uint32_t getTick(void){
	return tick;
}

uint32_t BSP_GetMicros(void) {
    uint32_t ms   = tick;
    uint32_t load = SysTick->LOAD + 1U;
    uint32_t val  = SysTick->VAL;
    return (ms * 1000U) + ((load - val) * 1000U / load);
}

void DelayMs(uint32_t delayMs){
	uint32_t startTick = getTick();
	
	while ((getTick() - startTick) < delayMs){}
	
}

uint8_t isTimeout(uint32_t startTick , uint32_t timeoutMs){
	return ((getTick() - startTick) >= timeoutMs);
}
	
