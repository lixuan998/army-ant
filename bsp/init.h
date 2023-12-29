#ifndef __INIT_H__
#define __INIT_H__

#define MUST        // Indicate that the prototype must be implemented.
#define SELECTIVE   // Indicate that the prototype can be unimplemented.

/****************************************************************************************************************************************************
 * All function prototypes without "Selective" must be implemented in init.c.
 * You are free to add your own function prototypes in this file.
****************************************************************************************************************************************************/

#include "arch/defs.h"
#include "console/include/console.h"
#include "driver/plic/plic.h"
#include "driver/timer/timer.h"
#include "driver/uart/uart.h"

/****************************************************************************************************************************************************
 * Serial I/O related functions.
****************************************************************************************************************************************************/
MUST void print_char(char c);
MUST void print_str(char *str);
MUST int scan_char();
#endif /* __INTI_H__ */