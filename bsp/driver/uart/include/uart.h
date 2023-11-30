#ifndef __UART_H__
#define __UART_H__

#define UART_BASE_ADDR 0x02500000
#define THR             0x00
#define USR             0x7C

void serial_print_char(char c);
void serial_print(char *msg);

#endif /* __UART_H__ */