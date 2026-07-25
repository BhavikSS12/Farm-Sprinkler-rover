#include "stm32f10x.h"

#include "BSP\bsp_systick.h"
#include "BSP\bsp_gpio.h"

int main(void)
{
    SystemInit();

    sysTick_Init();

    gpioInit();

    while(1)
    {
        led_toggle();

        DelayMs(500);
    }
}