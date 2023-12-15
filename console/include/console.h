#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "kernel/include/k_stdio.h"
#include "kernel/include/k_stdlib.h"
#include "kernel/include/k_string.h"
#include "kernel/include/k_logo.h"
#include "kernel/include/k_time.h"

void console_init();
void console_get_char(char c);


#endif /* __CONSOLE_H__ */