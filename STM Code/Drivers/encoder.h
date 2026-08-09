#ifndef ENCODER_H_
#define ENCODER_H_

#include <stdint.h>

//PA0 = Right encoder PA1 = Left encoder
//rising edge exti 

void encodeInit(void);
int32_t encoderReadA(void);
int32_t encoderReadB(void);
void encoderReset(void);

//exti0 //exti1 IQRHandler call functions
void encoder_ISR_A(void);
void encoder_ISR_B(void);

#endif