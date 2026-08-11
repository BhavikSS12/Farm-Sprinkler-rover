#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <stdint.h>

/* Feed one null-terminated line from UART to this function.
   It executes the appropriate motor/pump/encoder action. */
void CMD_Parse(const char *line);

#endif