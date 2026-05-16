#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <stdint.h>

void motorcontrol_init(void);
void driverA_Forward(uint8_t speed);
void driverA_Reverse(uint8_t speed);
void driverA_Stop(void);
void driverB_Forward(uint8_t speed);
void driverB_Reverse(uint8_t speed);
void driverB_Stop(void);
void driveForward(uint8_t speed);
void driveReverse(uint8_t speed);
void driveStop(void);
void turnLeft(uint8_t speed);
void turnRight(uint8_t speed);
void enableDrivers(void);
void disableDrivers(void);

#endif