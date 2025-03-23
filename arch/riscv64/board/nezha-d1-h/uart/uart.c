#include "register_defs.h"
#include <io/uart.h>
#include <common/stdint.h>
#include <common/operation.h>

void uart_init()
{
    // WRITE_REG32(UARTn_BASE(0) + UART_IER, ~UART_IER_ERBFI);
    // WRITE_REG32(UARTn_BASE(0) + UART_FCR, UART_FCR_ENABLE_FIFO);
    // WRITE_REG32(UARTn_BASE(0) + UART_LCR, UART_LCR_DLS_8);
    WRITE_REG32(UARTn_BASE(0) + UART_IER, UART_IER_ERBFI);
}

void uart_send_byte(uint8_t byte)
{
    while (1)
    {
        uint32_t lsr_val = READ_REG32(UARTn_BASE(0) + UART_LSR);
        if (lsr_val & (UART_LSR_THR_EMPTY))
        {
            break;
        }
    }
    volatile unsigned int *thr_val = (unsigned int *)(UARTn_BASE(0) + UART_THR);
    (*thr_val) = byte;
}

void uart_recv_byte(uint8_t *byte)
{
    if(READ_REG32(UARTn_BASE(0) + UART_LSR) & UART_LSR_DATA_READY)
    {
        *byte = (uint8_t)READ_REG32(UARTn_BASE(0) + UART_RBR);
    }
    else *byte = -1;
}

void uart_print_str(char *str)
{
    char *ptr = str;
    while((*ptr) != '\0')
    {
        uart_send_byte(*ptr);
        ptr ++;
    }
}