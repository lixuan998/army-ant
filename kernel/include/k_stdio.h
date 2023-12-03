#ifndef __K_STDIO_H__
#define __K_STDIO_H__

#include "k_stdarg.h"
#include "../include/k_defs.h"
#include "../../bsp/driver/uart/include/uart.h"

void k_printf(char *fmt, ...);
void k_panic(char *error);


#endif /* __K_STDIO_H__ */