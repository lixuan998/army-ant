#ifndef __GPIO_H__
#define __GPIO_H__

#include "arch/riscv/include/riscv_type_defs.h"
#include "arch/riscv/include/riscv_basic_operation.h"

typedef uint32                      (gpio_t);

#define GPIO_INT_CFG_POSITIVE_EDGE  (0)
#define GPIO_INT_CFG_NEGATIVE_EDGE  (1)
#define GPIO_INT_CFG_HIGH_LEVEL     (2)
#define GPIO_INT_CFG_LOW_LEVEL      (3)
#define GPIO_INT_CFG_DOUBLE_EDGE    (4)

#define GPIO_BASE_ADDR              (0x02000000)

#define GPIO_PA_CFG_BASE            (0x0)
#define GPIO_PB_CFG_BASE            (GPIO_BASE_ADDR + 0x0030)
#define GPIO_PC_CFG_BASE            (GPIO_BASE_ADDR + 0x0060)
#define GPIO_PD_CFG_BASE            (GPIO_BASE_ADDR + 0x0090)
#define GPIO_PE_CFG_BASE            (GPIO_BASE_ADDR + 0x00C0)
#define GPIO_PF_CFG_BASE            (GPIO_BASE_ADDR + 0x00F0)
#define GPIO_PG_CFG_BASE            (GPIO_BASE_ADDR + 0x0120)

#define GPIO_PA_DAT_BASE            (0x0)
#define GPIO_PB_DAT_BASE            (GPIO_BASE_ADDR + 0x0040)
#define GPIO_PC_DAT_BASE            (GPIO_BASE_ADDR + 0x0070)
#define GPIO_PD_DAT_BASE            (GPIO_BASE_ADDR + 0x00A0)
#define GPIO_PE_DAT_BASE            (GPIO_BASE_ADDR + 0x00D0)
#define GPIO_PF_DAT_BASE            (GPIO_BASE_ADDR + 0x0100)
#define GPIO_PG_DAT_BASE            (GPIO_BASE_ADDR + 0x0130)

#define GPIO_PA_DRV_BASE            (0x0)
#define GPIO_PB_DRV_BASE            (GPIO_BASE_ADDR + 0x0044)
#define GPIO_PC_DRV_BASE            (GPIO_BASE_ADDR + 0x0074)
#define GPIO_PD_DRV_BASE            (GPIO_BASE_ADDR + 0x00A4)
#define GPIO_PE_DRV_BASE            (GPIO_BASE_ADDR + 0x00D4)
#define GPIO_PF_DRV_BASE            (GPIO_BASE_ADDR + 0x0104)
#define GPIO_PG_DRV_BASE            (GPIO_BASE_ADDR + 0x0134)

#define GPIO_PA_PULL_BASE           (0x0)
#define GPIO_PB_PULL_BASE           (GPIO_BASE_ADDR + 0x0054)
#define GPIO_PC_PULL_BASE           (GPIO_BASE_ADDR + 0x0084)
#define GPIO_PD_PULL_BASE           (GPIO_BASE_ADDR + 0x00B4)
#define GPIO_PE_PULL_BASE           (GPIO_BASE_ADDR + 0x00E4)
#define GPIO_PF_PULL_BASE           (GPIO_BASE_ADDR + 0x0114)
#define GPIO_PG_PULL_BASE           (GPIO_BASE_ADDR + 0x0144)

#define GPIO_PA                     (0)
#define GPIO_PB                     (1)
#define GPIO_PC                     (2)
#define GPIO_PD                     (3)
#define GPIO_PE                     (4)
#define GPIO_PF                     (5)
#define GPIO_PG                     (6)

//normal gpio cfg.
typedef struct _N_GPIO_CFG{
    char port;
    char port_num;
    char func_select;
    char pull_state;
    char drv_level;
    char data;
    int data_read;
    char reserved[2];
} N_GPIO_CFG;

void gpio_config(N_GPIO_CFG *cfgs, int cnt);

void gpio_config_set(soc_reg_t cfg_reg, soc_reg_t cfg_val);
gpio_t gpio_config_read(soc_reg_t cfg_reg);
gpio_t gpio_data_read(soc_reg_t dat_reg);
void gpio_drv_set(soc_reg_t drv_reg, gpio_t val);
void gpio_pull_set(soc_reg_t pull_reg, gpio_t val);
void gpio_int_set(soc_reg_t eint_ctl_reg, gpio_t val);
void gpio_int_config(soc_reg_t eint_cfg_reg, gpio_t val);

#endif /* __GPIO_H__ */