#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f10x.h"

/* 4-bit GPIO mode/cnf values */
#define GPIO_IN_FLOAT    0x4U   /* Input floating          */
#define GPIO_IN_PUPD     0x8U   /* Input pull-up/pull-down */
#define GPIO_OUT_PP_2   0x2U   /* Output push-pull 2 MHz */
#define GPIO_AF_PP_50    0xBU   /* AF push-pull 50 MHz     */

void    gpioClockEnable(GPIO_TypeDef *port);
void    gpioInit(GPIO_TypeDef *port, uint8_t pin, uint8_t mode_cnf);
void    gpioWrite(GPIO_TypeDef *port, uint8_t pin, uint8_t state);
uint8_t gpioRead(GPIO_TypeDef *port, uint8_t pin);

void gpioSet(GPIO_TypeDef *port , uint8_t pin);
void gpioReset(GpioTypeDef *port , uint8_t pin);

#endif
