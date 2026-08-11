#include "bsp_uart.h"
#include "bsp_gpio.h"
#include "stm32f10x.h"

void uartInit(uint32_t baudRate){
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN | RCC_APB2ENR_AFIOEN;
	
	gpioInit(GPIOA , 9 , GPIO_AF_PP_50); //Tx
	gpioInit(GPIOA , 10 , GPIO_IN_FLOAT); //Rx
	
	USART1->BRR = (uint16_t)(SystemCoreClock / baudRate);
	
	USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE ;
}

void uartSendChar(char c){
	while(!(USART1->SR & USART_SR_TXE));
	
	USART1->DR = (uint8_t)c;
}

void uartSendStr(const char *s){
	while(*s) uartSendChar(*s++);
}

void uartSendInt(int32_t val){
	char buff[12];
	int i=0;
	
	if(val<0){
		uartSendChar('-');
		val = -val;
	}
	if(val == 0){
		uartSendChar('0');
		return;
	}
	
	while(val > 0 && i < 11){
		buff[i] = (char)('0' + (val % 10));
		val = val / 10;
		i++;
	}
	
	for(int j=i-1 ; j>=0 ; j--){
		uartSendChar(buff[j]);
	}
	
}

int8_t uartAvailable(void){
	return (USART1->SR & USART_SR_RXNE) ? 1U : 0U ;
}

char uartReadChar(void){
	while (!(USART1->SR & USART_SR_RXNE));
	
	return (char)(USART1->DR & 0xFFU);
}

uint8_t uartReadLine(char *buf , uint8_t maxlen){
	static uint8_t idx = 0;
	while(uartAvailable()){
		char c = uartReadChar();
		if(c == '\r') continue;
		if(c == '\n'){
			buf[idx] = '\0';
			idx = 0;
			return 1;
		}
		if(idx < maxlen - 1){
			buf[idx] = c;
			idx++;
		}
	}
	return 0;
}

