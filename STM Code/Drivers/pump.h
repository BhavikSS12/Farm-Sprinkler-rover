#ifndef PUMP_H
#define PUMP_H
#include <stdint.h>

/* R365 Pump via L298N
   IN1 = PA2 (direction HIGH)
   IN2 = PA3 (always LOW)
   ENA = PB3 (TIM2 CH2 PWM — timer-conflict-free) */

void Pump_Init(void);
void Pump_On(uint8_t speed);   /* 0–255 */
void Pump_Off(void);
#endif