#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

void encoder_init(void);
void encoderISR_A(void);
void encoderISR_B(void);
int32_t readPulsesA(void);
int32_t readPulsesB(void);
void resetEncoders(void);

#endif