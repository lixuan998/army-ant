#include <io/uart.h>
#include <lib/stdint.h>
#include <common/operation.h>

#include <common/lock.h>

/* UART related definitions. */
#define UART0_BASE           0x10000000L
#define UART0_IRQ            10
#define UARTn_BASE(n)        (UART ## n ## _BASE)
#define UART_THR             0x0000
#define UART_RBR             0x0000
#define UART_LSB             0x0000
#define UART_MSB             0x0001
#define UART_USR             0x007C
#define UART_IER             0x0001
#define UART_IER_ERBFI_MSK   (1UL << 0) // Enable Received Data Available Interrupt.
#define UART_IER_ERBFI_EN    (1UL << 0)
#define UART_IER_ETBEI_MSK   (1UL << 1) // Enable Transmitter Holding Register Empty Interrupt.
#define UART_IER_ETBEI_EN    (1UL << 1)
#define UART_FCR             0x0002
#define UART_FCR_FIFO_CLEAR  (3UL << 1)
#define UART_FCR_ENABLE_FIFO (1UL << 0)
#define UART_LCR             0x0003
#define UART_LCR_DLAB_MSK    (1UL << 7) // Divisor Latch Access Bit
#define UART_LCR_DLAB_EN     (1UL << 7) // Divisor Latch Access Bit
#define UART_LCR_DLS_MSK     (3UL << 0) // Data Length Select
#define UART_LCR_DLS_8       (3UL << 0)
#define UART_LSR             0x0005
#define UART_IIR             0x0008

#define UART_LSR_DATA_READY  1 << 0
#define UART_LSR_THR_EMPTY   1 << 5

#define UART_LCR_STOP        1 << 1
#define UART_LCR_PARITY      1 << 2


#define UART_IER_ERBFI       1 << 0 // Enable Received Data Available Interrupt. 

static spinlock_t _uart_splock;

void uart_init()
{
    WRITE_REG32(UARTn_BASE(0) + UART_IER, 0x00);
    WRITE_REG32(UARTn_BASE(0) + UART_LCR, UART_LCR_DLAB_EN);
    WRITE_REG32(UARTn_BASE(0) + UART_LSB, 0x03);
    WRITE_REG32(UARTn_BASE(0) + UART_MSB, 0x00);
    WRITE_REG32(UARTn_BASE(0) + UART_LCR, UART_LCR_DLS_8);
    WRITE_REG32(UARTn_BASE(0) + UART_FCR, UART_FCR_FIFO_CLEAR | UART_FCR_ENABLE_FIFO);
    WRITE_REG32(UARTn_BASE(0) + UART_IER, UART_IER_ERBFI_EN | UART_IER_ETBEI_EN);
    spinlock_init(&_uart_splock);
}

static void _uart_do_putc(char c)
{
    while (1) {
        uint32_t lsr_val = READ_REG32(UARTn_BASE(0) + UART_LSR);
        if (lsr_val & (UART_LSR_THR_EMPTY)) {
            break;
        }
        for(int i = 0; i < 10000; i++);
    }
    WRITE_REG32(UARTn_BASE(0) + UART_THR, c);
}

void uartputc(char c)
{
    spinlock_lock(&_uart_splock);
    _uart_do_putc(c);
    spinlock_unlock(&_uart_splock);
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