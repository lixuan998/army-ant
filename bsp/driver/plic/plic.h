#ifndef __PLIC_H__
#define __PLIC_H__

#include "arch/defs.h"
#include "kernel/include/k_stdio.h"

#define PLIC_ENABLE                 (1)
#define PLIC_DISABLE                (0)

#define PLIC_BASE_ADDR              (0x10000000UL)
#define PLIC_PRIO_REG(n)            (PLIC_BASE_ADDR + 0x0000UL + 0x0004UL * n)   //PLIC Priority Register n.
#define PLIC_IP_REG(n)              (PLIC_BASE_ADDR + 0x1000UL + 0x0004UL * n)   //PLIC Interrupt Pending Register n.
#define PLIC_MIE_REG(n)             (PLIC_BASE_ADDR + 0x2000UL + 0x0004UL * n)   //PLIC Machine Mode Interrupt Enable Register n.
#define PLIC_SIE_REG(n)             (PLIC_BASE_ADDR + 0x2080UL + 0x0004UL * n)   //PLIC Superuser Mode Interrupt Enable Register n.
#define PLIC_CTRL_REG               (PLIC_BASE_ADDR + 0x1FFFFCUL)              //PLIC Control Register.
#define PLIC_MTH_REG                (PLIC_BASE_ADDR + 0x200000UL)              //PLIC Machine Threshold Register.
#define PLIC_MCLAIM_REG             (PLIC_BASE_ADDR + 0x200004UL)              //PLIC Machine Claim Register.
#define PLIC_STH_REG                (PLIC_BASE_ADDR + 0x201000UL)              //PLIC Superuser Threshold Register.
#define PLIC_SCLAIM_REG             (PLIC_BASE_ADDR + 0x201004UL)              //PLIC Superuser Claim Register.

#define PLIC_CTRL_REG_S_MODE_ACCESS 1UL

#define PLIC_INTERRUPT_MAX          256

enum PLIC_INTERRUPT_SOURCE{
    UART0               = 18,
    UART1               = 19,
    UART2               = 20,
    UART3               = 21,
    UART4               = 22,
    UART5               = 23,
    TWI0                = 25,
    TWI1                = 26,
    TWI2                = 27,
    TWI3                = 28,
    SPI0                = 31,
    SPI1                = 32,
    PWM                 = 34,
    IR_TX               = 35,
    LEDC                = 36,
    OWA                 = 39,
    DMIC                = 40,
    AUDIO_CODEC         = 41,
    I2CPCM0             = 42,
    I2CPCM1             = 43,
    I2CPCM2             = 44,
    USB0_DEVICE         = 45,
    USB0_EHCI           = 46,
    USB0_OHCI           = 47,
    USB1_EHCI           = 49,
    USB1_OHCI           = 50,
    SMHC0               = 56,
    SMHC1               = 57,
    SMHC2               = 58,
    MSI                 = 59,
    EMAC                = 62,
    CCU_FERR            = 64,
    AHB_HREADY_TIME_OUT = 65,
    DMAC_NS             = 66,
    CE_NS               = 68,
    SPINLOCK            = 70,
    HSTIMER0            = 71,
    HSTIMER1            = 72,
    GPADC               = 73,
    THS                 = 74,
    TIMER0              = 75,
    TIMER1              = 76,
    LRADC               = 77,
    TPADC               = 78,
    WATCHDOG            = 79,
    IOMMU               = 80,
    VE                  = 82,
    GPIOB_NS            = 85,
    GPIOC_NS            = 87,
    GPIOD_NS            = 89,
    GPIOE_NS            = 91,
    GPIOF_NS            = 93,
    GPIOG_NS            = 95,
    DE                  = 103,
    DI                  = 104,
    G2D                 = 105,
    LCD                 = 106,
    TV                  = 107,
    DSI                 = 108,
    HDMI                = 109,
    TVE                 = 110,
    CSI_DMA0            = 111,
    CSI_DMA1            = 112,
    CSI_PARSER0         = 116,
    CSI_TOP_PKT         = 122,
    TVD                 = 123,
    DSP_DEE             = 136,
    DSP_PFE             = 137,
    DSP_WDG             = 138,
    DSP_MBOX_RISCV_W    = 140,
    DSP_TZMA            = 141,
    DMAC_IRQ_DSP_NS     = 142,
    RISCV_MBOX_RISCV    = 144,
    RISCV_MBOX_DSP      = 145,
    RISCV_WDG           = 147,
    IRRX                = 167,
    C0_CTI0             = 176,
    C0_CTI1             = 177,
    C0_COMMTX0          = 180,
    C0_COMMTX1          = 181,
    C0_COMMRX0          = 184,
    C0_COMMRX1          = 185,
    C0_PMU0             = 188,
    C0_PMU1             = 189,
    C0_AXI_ERROR        = 192,
    AXI_WR_IRQ          = 194,
    AXI_RD_IRQ          = 195
};

void plic_s_mode_access();
void plic_interrupt_enable();
void plic_interrupt_disable();
#endif /* __PLIC_H__ */