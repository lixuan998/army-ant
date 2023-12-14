#ifndef __K_STDIO_H__
#define __K_STDIO_H__

#include "k_stdarg.h"
#include "kernel/include/k_defs.h"
#include "bsp/driver/uart/uart.h"

void k_printf(char *fmt, ...);
void k_sprintf(char *str, char *fmt, ...);
int k_getc(char *c);
void k_putc(char c);
void k_panic(char *file_name, int line, char *error);


#endif /* __K_STDIO_H__ */