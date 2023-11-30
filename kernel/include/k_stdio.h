#ifndef __K_STDIO_H__
#define __K_STDIO_H__

#include "k_stdarg.h"
#include "../../bsp/driver/uart/include/uart.h"

void k_printf(char *fmt, ...)
{
    va_list arg_list;
    va_start(arg_list, fmt);
    while(*fmt != '\0')
    {
        if((*fmt == '%'))
        {
            fmt ++;
            if((*fmt == 'd'))
            {
                int val = va_arg(arg_list, int);
                char buff[100];
                int idx = -1;
                if(val == 0) buff[++idx] = '0';
                while(val != 0)
                {
                    buff[++idx] = (val % 10) + '0';
                    val /= 10;
                }
                while(idx >= 0)
                {
                    serial_print_char(buff[idx--]);
                }
            }
            else if((*fmt) == 's')
            {
                char *str = va_arg(arg_list, char *);
                serial_print(str);
            }
            else if((*fmt) == 'c')
            {
                char ch = va_arg(arg_list, int);
                serial_print_char(ch);
            }
            else if((*fmt) == 'x')
            {
                int val = va_arg(arg_list, int);
                char buff[100];
                int idx = -1;
                if(val == 0) buff[++idx] = '0';
                while(val != 0)
                {
                    buff[++idx] = (val % 16) + '0';
                    val /= 16;
                }
                buff[++idx] = 'x';
                buff[++idx] = '0';
                while(idx >= 0)
                {
                    serial_print_char(buff[idx--]);
                }
            }
        }
        else
        {
            serial_print_char(*fmt);
        }
        ++ fmt;
    }
}

#endif /* __K_STDIO_H__ */