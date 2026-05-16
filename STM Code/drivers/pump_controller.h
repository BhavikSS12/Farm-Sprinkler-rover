#ifndef PUMPCONTROL_H
#define PUMPCONTROL_H

#include <stdint.h>

void pumpcontrol_init(void);
void pumpOn(uint8_t speed);
void pumpOff(void);

#endif