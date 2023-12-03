#include "../include/uart.h"

void serial_print_char(char c)
{
    #ifndef DEBUG
    while (1)
    {
        volatile unsigned int *usr_val = (unsigned int *)(UART_BASE_ADDR + USR);
        if (!((*usr_val) & 1))
        {
            break;
        }
    }
    #endif
    volatile unsigned int *thr_val = (unsigned int *)(UART_BASE_ADDR + THR);
    (*thr_val) = c;
}

void serial_print(char *msg)
{
    char *ptr = msg;
    while((*ptr) != '\0')
    {
        serial_print_char(*ptr);
        ptr ++;
    }
}