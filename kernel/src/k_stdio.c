#include "kernel/include/k_stdio.h"

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
                int negative = 0;
                if(val < 0)
                {
                    negative = 1;
                    val = -val;
                }
                char buff[100];
                int idx = -1;
                if(val == 0) buff[++idx] = '0';
                while(val != 0)
                {
                    buff[++idx] = (val % 10) + '0';
                    val /= 10;
                }
                if(negative) buff[++idx] = '-';
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
                char hex[] = "0123456789ABCDEF";
                uint64 val = va_arg(arg_list, uint64);
                char buff[100];
                int idx = -1;
                if(val == 0) buff[++idx] = '0';
                while(val != 0)
                {
                    buff[++idx] = hex[(val % 16)];
                    val /= 16UL;
                }
                buff[++idx] = 'x';
                buff[++idx] = '0';
                while(idx >= 0)
                {
                    serial_print_char(buff[idx--]);
                }
            }
            else if((*fmt) == 'p')
            {
                char hex[] = "0123456789ABCDEF";
                uint64 val = va_arg(arg_list, uint64);
                char buff[100];
                int idx = -1;
                if(val == 0) buff[++idx] = '0';
                while(val != 0)
                {
                    buff[++idx] = hex[(val % 16)];
                    val /= 16UL;
                }
                buff[++idx] = 'x';
                buff[++idx] = '0';
                while(idx >= 0)
                {
                    serial_print_char(buff[idx--]);
                }
            }
            else if((*fmt) == '%')
            {
                serial_print_char('%');
            }
        }
        else
        {
            serial_print_char(*fmt);
        }
        ++ fmt;
    }
}

void k_sprintf(char *str, char *fmt, ...)
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
                int negative = 0;
                if(val < 0)
                {
                    negative = 1;
                    val = -val;
                }
                char buff[100];
                int idx = -1;
                if(val == 0) buff[++idx] = '0';
                while(val != 0)
                {
                    buff[++idx] = (val % 10) + '0';
                    val /= 10;
                }
                if(negative) buff[++idx] = '-';
                while(idx >= 0)
                {
                    *(str ++) = (buff[idx--]);
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
                char hex[] = "0123456789ABCDEF";
                uint64 val = va_arg(arg_list, uint64);
                char buff[100];
                int idx = -1;
                if(val == 0) buff[++idx] = '0';
                while(val != 0)
                {
                    buff[++idx] = hex[(val % 16)];
                    val /= 16UL;
                }
                buff[++idx] = 'x';
                buff[++idx] = '0';
                while(idx >= 0)
                {
                    *(str ++) = (buff[idx--]);
                }
            }
            else if((*fmt) == 'p')
            {
                char hex[] = "0123456789ABCDEF";
                uint64 val = va_arg(arg_list, uint64);
                char buff[100];
                int idx = -1;
                if(val == 0) buff[++idx] = '0';
                while(val != 0)
                {
                    buff[++idx] = hex[(val % 16)];
                    val /= 16UL;
                }
                buff[++idx] = 'x';
                buff[++idx] = '0';
                while(idx >= 0)
                {
                    *(str ++) = (buff[idx--]);
                }
            }
            else if((*fmt) == '%')
            {
                *(str ++) = '%';
            }
        }
        else
        {
            *(str ++) = *fmt;
        }
        ++ fmt;
    }
    *str = '\0';
}

int k_getc(char *c)
{
    int val = serial_receive_data();
    if(val < 0) return -1;
    *c = (char)val;
    return 0;
}

void k_putc(char c)
{
    if(c == '\r') k_printf("\n");
    k_printf("%c", c);
}

void k_panic(char *file_name, int line, char *error)
{
    k_printf("%s[%d]error: %s\n\r", file_name, line, error);
    k_printf("kernel freezed...");
    while(1);
}