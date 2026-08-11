#ifndef UART_H_
#define UART_H_

#include "stdint.h"

/* USART1: PA9=TX, PA10=RX, 8N1 */
void uartInit(uint32_t baudRate);
void uartSendChar(char c);
void uartSendStr(const char *s);
void uartSendInt(int32_t val);
int8_t uartAvailable(void);

char uartReadChar(void);

/* Non-blocking line reader. Returns 1 when '\n' received. */
uint8_t uartReadLine(char *buf , uint8_t maxlen);

#endif