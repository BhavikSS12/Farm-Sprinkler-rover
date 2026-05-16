#ifndef SERIALCMD_H
#define SERIALCMD_H

#include <stdint.h>

void serialcmd_init(void);
void serialCmd_poll(void);
void serialCmd_rxByte(uint8_t byte);

#endif