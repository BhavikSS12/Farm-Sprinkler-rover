#ifndef MOTOR_H
#define MOTOR_H
#include <stdint.h>

/* Drive motors — BTS7960 dual H-bridge
   Driver A = Left side (TIM3 CH1/CH2 → PB4, PB5)
   Driver B = Right side (TIM4 CH1/CH2 → PB6, PB7)
   Enable lines: R_EN_A=PB0, L_EN_A=PB1, R_EN_B=PB8, L_EN_B=PB9 */

void Motor_Init(void);
void Motor_EnableAll(void);
void Motor_DisableAll(void);

void Motor_A_Forward(uint8_t speed);
void Motor_A_Reverse(uint8_t speed);
void Motor_A_Stop(void);

void Motor_B_Forward(uint8_t speed);
void Motor_B_Reverse(uint8_t speed);
void Motor_B_Stop(void);

void Motor_Forward(uint8_t speed);
void Motor_Reverse(uint8_t speed);
void Motor_TurnLeft(uint8_t speed);
void Motor_TurnRight(uint8_t speed);
void Motor_Stop(void);
#endif