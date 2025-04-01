#pragma once

#include <lib/stdint.h>
#include <lib/stddef.h>

void uart_init();
void uartputc(char c);
void uartgetc(uint8_t *byte);
void uartputs(char *str);