#include "uart.h"

void uart_init()
{
    // write32(UART_BASE_ADDR + UART_IER, ~UART_IER_ERBFI);
    // write32(UART_BASE_ADDR + UART_FCR, UART_FCR_ENABLE_FIFO);
    // write32(UART_BASE_ADDR + UART_LCR, UART_LCR_DLS_8);
    write32(UART_BASE_ADDR + UART_IER, UART_IER_ERBFI);
}

void uart_print_char(char c)
{
    while (1)
    {
        uint32 lsr_val = read32(UART_BASE_ADDR + UART_LSR); // 获取LSR寄存器的值
        if (lsr_val & (UART_LSR_THR_EMPTY))
        {
            break;
        }
    }
    volatile unsigned int *thr_val = (unsigned int *)(UART_BASE_ADDR + UART_THR); // 获取THR寄存器的地址
    (*thr_val) = c; // 将数据写入THR寄存器
}

int uart_scan_char()
{
    if(read32(UART_BASE_ADDR + UART_LSR) & UART_LSR_DATA_READY)
    {
        return (int)read32(UART_BASE_ADDR + UART_RBR); // 读取RBR寄存器中的数据
    }
    else return -1;
}

void uart_print_str(char *str)
{
    char *ptr = str;
    while((*ptr) != '\0')
    {
        uart_print_char(*ptr);
        ptr ++;
    }
}