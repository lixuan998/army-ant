#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "kernel/include/k_stdio.h"
#include "kernel/include/k_stdlib.h"
#include "kernel/include/k_string.h"
#include "kernel/include/k_logo.h"

#define COMMAND_BUFF_SIZE 1024

#define DEL_KEY           127

void console_init();
void console_get_char(char c);

// Remove special charactors from command, and echo to the terminal.
void console_cmd_process();

#endif /* __CONSOLE_H__ */