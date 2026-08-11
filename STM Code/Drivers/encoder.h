#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint.h>

//PA0 = Right encoder PA1 = Left encoder
//rising edge exti 

void Encoder_Init(void);
int32_t Encoder_ReadA(void);
int32_t Encoder_ReadB(void);
void Encoder_Reset(void);

//exti0 //exti1 IRQHandler call functions
void Encoder_ISR_A(void);
void Encoder_ISR_B(void);

#endif