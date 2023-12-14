#include "uart.h"

void uart_init()
{
    write_reg32(UART_BASE_ADDR + UART_IER, 0);
    write_reg32(UART_BASE_ADDR + UART_FCR, 1);
    write_reg32(UART_BASE_ADDR + UART_LCR, 3);
    write_reg32(UART_BASE_ADDR + UART_IER, 1);
}

void serial_print_char(char c)
{
    #ifndef DEBUG
    while (1)
    {
        uint32 lsr_val = read_reg32(UART_BASE_ADDR + UART_LSR);
        if (lsr_val & (1 << 5))
        {
            break;
        }
    }
    #endif
    volatile unsigned int *thr_val = (unsigned int *)(UART_BASE_ADDR + UART_THR);
    (*thr_val) = c;
}

int serial_receive_data()
{
    if(read_reg32(UART_BASE_ADDR + UART_LSR) & UART_LSR_DATA_READY)
    {
        return (int)read_reg32(UART_BASE_ADDR + UART_RBR);
    }
    else return -1;
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