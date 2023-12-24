#ifndef __UART_H__
#define __UART_H__

#include "arch/defs.h"


#define UART_BASE_ADDR       0x04140000
#define UART_THR             0x0000
#define UART_RBR             0x0000
#define UART_USR             0x007C
#define UART_IER             0x0004
#define UART_FCR             0x0008
#define UART_LCR             0x000C
#define UART_LSR             0x0014
#define UART_IIR             0x0008

#define UART_LSR_DATA_READY  1 << 0
void uart_init();

void serial_print_char(char c);
int serial_receive_data();
void serial_print(char *msg);

#endif /* __UART_H__ */