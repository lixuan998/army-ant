#ifndef __STDIO_H__
#define __STDIO_H__

#define UART_STDIO

#include "stdarg.h"
#include "bsp/driver/uart/uart.h"

#define panic(str) _panic(__FILE__, __LINE__, str);

void printf(char *fmt, ...);
void sprintf(char *str, char *fmt, ...);
int getc(char *c);
void putc(char c);
void _panic(char *file_name, int line, char *error);


#endif /* __STDIO_H__ */