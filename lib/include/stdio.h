#ifndef __STDIO_H__
#define __STDIO_H__

#include "stdarg.h"
#include "kernel/include/k_defs.h"
#include "bsp/init.h"

void printf(char *fmt, ...);
void sprintf(char *str, char *fmt, ...);
int getc(char *c);
void putc(char c);
void panic(char *file_name, int line, char *error);


#endif /* __STDIO_H__ */