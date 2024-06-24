#include "../include/stdio.h"

#ifdef UART_STDIO
void (*print_char)(char) = uart_print_char;
void (*print_str)(char *) = uart_print_str;
int (*scan_char)() = uart_scan_char;
#endif  /* UART_STDIO */
 
void printf(char *fmt, ...)
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
                    print_char(buff[idx--]);
                }
            }
            else if((*fmt) == 's')
            {
                char *str = va_arg(arg_list, char *);
                print_str(str);
            }
            else if((*fmt) == 'c')
            {
                char ch = va_arg(arg_list, int);
                print_char(ch);
            }
            else if((*fmt) == 'x')
            {
                char hex[] = "0123456789ABCDEF";
                ptr_t val = va_arg(arg_list, ptr_t);
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
                    print_char(buff[idx--]);
                }
            }
            else if((*fmt) == 'p')
            {
                char hex[] = "0123456789ABCDEF";
                ptr_t val = va_arg(arg_list, ptr_t);
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
                    print_char(buff[idx--]);
                }
            }
            else if((*fmt) == 'b')
            {
                ptr_t val = va_arg(arg_list, ptr_t);
                // if(val == 0) buff[++idx] = '0';
                // while(val != 0)
                // {
                //     buff[++idx] = '0' + (val & 1);
                //     val >>= 1;
                // }
                print_char('b');
                // while(idx >= 0)
                // {
                //     print_char(buff[idx--]);
                // }
                for(int i = 31; i >= 0; --i)
                {
                    if(val & (1UL << i)) print_char('1');
                    else print_char('0'); 
                    // print_char('0' + (val & (1 << i)));
                }
            }
            else if((*fmt) == '%')
            {
                print_char('%');
            }
        }
        else
        {
            print_char(*fmt);
        }
        ++ fmt;
    }
}

void sprintf(char *str, char *fmt, ...)
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
                print_str(str);
            }
            else if((*fmt) == 'c')
            {
                char ch = va_arg(arg_list, int);
                print_char(ch);
            }
            else if((*fmt) == 'x')
            {
                char hex[] = "0123456789ABCDEF";
                ptr_t val = va_arg(arg_list, ptr_t);
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
                ptr_t val = va_arg(arg_list, ptr_t);
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

int getc(char *c)
{
    int val = scan_char();
    if(val < 0) return -1;
    *c = (char)val;
    return 0;
}

void putc(char c)
{
    if(c == '\r') printf("\n");
    printf("%c", c);
}

void _panic(char *file_name, int line, char *error)
{
    printf("%s[%d] error: %s\n\r", file_name, line, error);
    printf("kernel freezed...");
    while(1);
}