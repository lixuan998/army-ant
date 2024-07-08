#ifndef __PLIC_H__
#define __PLIC_H__

#include "arch/riscv/include/riscv_type_defs.h"
#include "arch/riscv/include/riscv_basic_operation.h"
#include "lib/include/stdio.h"

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

enum PLIC_EXTERNAL_INTERRUPT_SOURCE{
    UART0_SOURCE               = 18,
    UART1_SOURCE               = 19,
    UART2_SOURCE               = 20,
    UART3_SOURCE               = 21,
    UART4_SOURCE               = 22,
    UART5_SOURCE               = 23,
    TWI0_SOURCE                = 25,
    TWI1_SOURCE                = 26,
    TWI2_SOURCE                = 27,
    TWI3_SOURCE                = 28,
    SPI0_SOURCE                = 31,
    SPI1_SOURCE                = 32,
    PWM_SOURCE                 = 34,
    IR_TX_SOURCE               = 35,
    LEDC_SOURCE                = 36,
    OWA_SOURCE                 = 39,
    DMIC_SOURCE                = 40,
    AUDIO_CODEC_SOURCE         = 41,
    I2CPCM0_SOURCE             = 42,
    I2CPCM1_SOURCE             = 43,
    I2CPCM2_SOURCE             = 44,
    USB0_DEVICE_SOURCE         = 45,
    USB0_EHCI_SOURCE           = 46,
    USB0_OHCI_SOURCE           = 47,
    USB1_EHCI_SOURCE           = 49,
    USB1_OHCI_SOURCE           = 50,
    SMHC0_SOURCE               = 56,
    SMHC1_SOURCE               = 57,
    SMHC2_SOURCE               = 58,
    MSI_SOURCE                 = 59,
    EMAC_SOURCE                = 62,
    CCU_FERR_SOURCE            = 64,
    AHB_HREADY_TIME_OUT_SOURCE = 65,
    DMAC_NS_SOURCE             = 66,
    CE_NS_SOURCE               = 68,
    SPINLOCK_SOURCE            = 70,
    HSTIMER0_SOURCE            = 71,
    HSTIMER1_SOURCE            = 72,
    GPADC_SOURCE               = 73,
    THS_SOURCE                 = 74,
    TIMER0_SOURCE              = 75,
    TIMER1_SOURCE              = 76,
    LRADC_SOURCE               = 77,
    TPADC_SOURCE               = 78,
    WATCHDOG_SOURCE            = 79,
    IOMMU_SOURCE               = 80,
    VE_SOURCE                  = 82,
    GPIOB_NS_SOURCE            = 85,
    GPIOC_NS_SOURCE            = 87,
    GPIOD_NS_SOURCE            = 89,
    GPIOE_NS_SOURCE            = 91,
    GPIOF_NS_SOURCE            = 93,
    GPIOG_NS_SOURCE            = 95,
    DE_SOURCE                  = 103,
    DI_SOURCE                  = 104,
    G2D_SOURCE                 = 105,
    LCD_SOURCE                 = 106,
    TV_SOURCE                  = 107,
    DSI_SOURCE                 = 108,
    HDMI_SOURCE                = 109,
    TVE_SOURCE                 = 110,
    CSI_DMA0_SOURCE            = 111,
    CSI_DMA1_SOURCE            = 112,
    CSI_PARSER0_SOURCE         = 116,
    CSI_TOP_PKT_SOURCE         = 122,
    TVD_SOURCE                 = 123,
    DSP_DEE_SOURCE             = 136,
    DSP_PFE_SOURCE             = 137,
    DSP_WDG_SOURCE             = 138,
    DSP_MBOX_RISCV_W_SOURCE    = 140,
    DSP_TZMA_SOURCE            = 141,
    DMAC_IRQ_DSP_NS_SOURCE     = 142,
    RISCV_MBOX_RISCV_SOURCE    = 144,
    RISCV_MBOX_DSP_SOURCE      = 145,
    RISCV_WDG_SOURCE           = 147,
    IRRX_SOURCE                = 167,
    C0_CTI0_SOURCE             = 176,
    C0_CTI1_SOURCE             = 177,
    C0_COMMTX0_SOURCE          = 180,
    C0_COMMTX1_SOURCE          = 181,
    C0_COMMRX0_SOURCE          = 184,
    C0_COMMRX1_SOURCE          = 185,
    C0_PMU0_SOURCE             = 188,
    C0_PMU1_SOURCE             = 189,
    C0_AXI_ERROR_SOURCE        = 192,
    AXI_WR_IRQ_SOURCE          = 194,
    AXI_RD_IRQ_SOURCE          = 195
};


void plic_s_mode_access();
void plic_interrupt_enable();
void plic_interrupt_disable();
int plic_interrupt_source();
void plic_interrupt_handled(int source);
#endif /* __PLIC_H__ */