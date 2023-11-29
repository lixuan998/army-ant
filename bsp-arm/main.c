/*
 *配置串口传输主程序
 *S3C2440
 *
 */
#include "../kernel/include/k_logo.h"
#define GPHCON (*(unsigned int *)(0x56000070))
#define ULCON0 (*(unsigned int *)(0x50000000))
#define UCON0 (*(unsigned int *)(0x50000004))
#define UBRDIV0 (*(unsigned int *)(0x50000028))
#define UTXH0 (*(unsigned int *)(0x50000020))
#define URXH0 (*(unsigned int *)(0x50000024))
#define UTRSTAT0 (*(unsigned int *)(0x50000010))

#define UART0_BASE_ADDR 0x01C28000
#define THR             0x00
#define USR             0x7C


void configGPH(void);

void configULC(void);

void configUC(void);

void configUBR(void);

void uart_init(void);

void RTX0(void)
{
    while (1)
    {
        volatile unsigned int *usr_val = (unsigned int *)(UART0_BASE_ADDR + USR);
        if (!((*usr_val) & 1))
        {
            break;
        }
    }

    volatile unsigned int *thr_val = (unsigned int *)(UART0_BASE_ADDR + THR);
    (*thr_val) = 'c';

}

void serial_print_char(char c)
{
    while (1)
    {
        volatile unsigned int *usr_val = (unsigned int *)(UART0_BASE_ADDR + USR);
        if (!((*usr_val) & 1))
        {
            break;
        }
    }
    volatile unsigned int *thr_val = (unsigned int *)(UART0_BASE_ADDR + THR);
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

int main(void)
{
    // uart_init();
    serial_print(ARMY_ANT_LOGO);
    // serial_print("end\n\r");
    // RTX0();
    while(1);
    return 0;
}

void uart_init(void)
{
    configGPH();
    configULC();
    configUC();
    configUBR();
}

void configGPH(void)  //设置GPH2、3分别为TXD0,RXD0
{
    unsigned int GPH23 = GPHCON;
    GPH23 |= 1 << 5;
    GPH23 |= 1 << 3;
    GPH23 &= ~(1 << 4);
    GPH23 &= ~(1 << 2);
    GPHCON = GPH23;
}

void configULC(void)  //设置传输字长为8bit
{
    unsigned int ULC = ULCON0;
    ULC |= 1 << 1;
    ULC |= 1;
    ULCON0 = ULC;
}

void configUC(void)  //设置接受与发送为polling模式
{
    unsigned int UC = UCON0;
    UC |= (1 << 2);
    UC |= 1;
    UC &= ~(1 << 3);
    UC &= ~(1 << 1);
    UCON0 = UC;
}

/*
 * 设置波特率 寄存器设置为（时钟频率50MHZ）/（波特率115200 * 16）-1 最后为26
 */
void configUBR(void)
{
    unsigned int UBR = UBRDIV0;
    UBR = 26;
    UBRDIV0 = UBR;
}
