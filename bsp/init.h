#ifndef __INIT_H__
#define __INIT_H__

#define MUST        // Indicate that the prototype must be implemented.
#define SELECTIVE   // Indicate that the prototype can be unimplemented.

/****************************************************************************************************************************************************
 * All function prototypes without "Selective" must be implemented in init.c.
 * You are free to add your own function prototypes in this file.
****************************************************************************************************************************************************/

#include "arch/defs.h"
#include "console/include/console.h"
#include "driver/plic/plic.h"
#include "driver/timer/timer.h"
#include "driver/uart/uart.h"
#include "arch/riscv/include/riscv_asm_operation.h"

MUST enum EXTERNAL_INTERRUPT_SOURCE{
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

/****************************************************************************************************************************************************
 * Spinlock related functions.
****************************************************************************************************************************************************/
MUST void spinlock_init(SPINLOCK *slk, char *name);
MUST void spinlock_lock(SPINLOCK *slk);
MUST void spinlock_unlock(SPINLOCK *slk);

/****************************************************************************************************************************************************
 * Memory Paging related functions.
****************************************************************************************************************************************************/
MUST void *alloc_single_page();
MUST void free_single_page(void *page);

/****************************************************************************************************************************************************
 * Interrupt related functions.
****************************************************************************************************************************************************/
MUST void interrupt_init();
MUST void interrupt_enable();
MUST void interrupt_disable();
MUST void interrupt_handler();
SELECTIVE void external_interrupt_handler();
SELECTIVE void external_interrupt_handled(int source);
SELECTIVE void timer_interrupt_handler();
SELECTIVE void software_interrupt_handler();

/****************************************************************************************************************************************************
 * Virtual Memory related functions.
****************************************************************************************************************************************************/
MUST int vm_map(pagetable_t pagetable, addr_t virt_addr_start, addr_t phys_addr_start, pagesize_t size, isa_reg_t permisson);
SELECTIVE pte_t* pte_retrieve(pagetable_t pagetable, addr_t virt_addr);
MUST void set_vm_pagetable(pagetable_t pagetable);

/****************************************************************************************************************************************************
 * Serial I/O related functions.
****************************************************************************************************************************************************/
MUST void print_char(char c);
MUST void print_str(char *str);
MUST int scan_char();
#endif /* __INTI_H__ */