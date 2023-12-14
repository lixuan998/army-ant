#include "../include/console.h"

char cmd_buf[COMMAND_BUFF_SIZE];
int cmd_buf_idx = 0;

void console_init()
{
    cmd_buf_idx = 0;
    k_memset(cmd_buf, '\0', COMMAND_BUFF_SIZE);
    k_printf(ARMY_ANT_LOGO);
    k_printf("$ ");
}

void console_get_char(char c)
{
    k_printf("c: %d\n\r", (int)c);
    cmd_buf[cmd_buf_idx ++] = c;
}

void console_cmd_process()
{
    
}