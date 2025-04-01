#include "register_defs.h"
#include <io/uart.h>
#include <lib/stdint.h>
#include <common/operation.h>

void uart_init()
{
    // WRITE_REG32(UARTn_BASE(0) + UART_IER, ~UART_IER_ERBFI);
    // WRITE_REG32(UARTn_BASE(0) + UART_FCR, UART_FCR_ENABLE_FIFO);
    // WRITE_REG32(UARTn_BASE(0) + UART_LCR, UART_LCR_DLS_8);
    WRITE_REG32(UARTn_BASE(0) + UART_IER, UART_IER_ERBFI);
}

void uartputc(char c)
{
    while (1)
    {
        uint32_t lsr_val = READ_REG32(UARTn_BASE(0) + UART_LSR);
        if (lsr_val & (UART_LSR_THR_EMPTY))
        {
            break;
        }
    }
    WRITE_REG32(UARTn_BASE(0) + UART_THR, c);
}

void uartgetc(uint8_t *byte)
{
    if(READ_REG32(UARTn_BASE(0) + UART_LSR) & UART_LSR_DATA_READY)
    {
        *byte = (uint8_t)READ_REG32(UARTn_BASE(0) + UART_RBR);
    }
    else *byte = -1;
}

void uartputs(char *str)
{
    char *ptr = str;
    while((*ptr) != '\0')
    {
        uartputc(*ptr);
        ptr ++;
    }
}