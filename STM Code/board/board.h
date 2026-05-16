#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

void board_init(void);
void delay_ms(uint32_t ms);
uint32_t micros(void);
void uart1_putc(char c);
void uart1_puts(const char *s);
void led_on(void);
void led_off(void);

#endif