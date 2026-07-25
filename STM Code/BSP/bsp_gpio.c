#include "bsp_gpio.h"

void gpioClockEnable(GPIO_TypeDef *port){
	if(port == GPIOA){
		RCC->APB2ENR = RCC_APB2ENR_IOPAEN;
	}
	else if(port == GPIOB){
		RCC->APB2ENR = RCC_APB2ENR_IOPBEN;
	}
	else if(port == GPIOC){
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	}
	else if(port == GPIOD){
		RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	}
}

void gpioInit(GPIO_TypeDef *port , uint8_t pin , uint8_t mode_cnf){
	if (pin < 8) {
        uint32_t sh = (uint32_t)pin * 4U;
        port->CRL = (port->CRL & ~(0xFU << sh)) | ((uint32_t)mode_cnf << sh);
    } else {
        uint32_t sh = ((uint32_t)pin - 8U) * 4U;
        port->CRH = (port->CRH & ~(0xFU << sh)) | ((uint32_t)mode_cnf << sh);
    }
}

void gpioWrite(GPIO_TypeDef *port , uint8_t pin , uint8_t state){
	port->BSRR = state ? (1U << pin):(1U << (pin + 16U));
}

uint8_t gpioRead(GPIO_TypeDef *port , uint8_t pin){
	return (uint8_t)((port->IDR >> pin) & 1U ); 
}


