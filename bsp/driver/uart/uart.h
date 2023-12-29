#ifndef __UART_H__
#define __UART_H__

#include "arch/defs.h"


#define UART_BASE_ADDR       0x02500000
#define UART_THR             0x0000
#define UART_RBR             0x0000
#define UART_USR             0x007C
#define UART_IER             0x0004
#define UART_FCR             0x0008
#define UART_LCR             0x000C
#define UART_LSR             0x0014
#define UART_IIR             0x0008

#define UART_LSR_DATA_READY  1 << 0
#define UART_LSR_THR_EMPTY   1 << 5

#define UART_LCR_DLS_8       3 << 0
#define UART_LCR_STOP        1 << 1
#define UART_LCR_PARITY      1 << 2

#define UART_FCR_ENABLE_FIFO 1 << 0

#define UART_IER_ERBFI       1 << 0 // Enable Received Data Available Interrupt. 
void uart_init();

void uart_print_char(char c);
int uart_scan_char();
void uart_print_str(char *str);

#endif /* __UART_H__ */