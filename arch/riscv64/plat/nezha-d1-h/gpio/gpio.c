#include "gpio.h"

void gpio_config(N_GPIO_CFG *cfgs, int cnt)
{
    for(int i=0; i<cnt; i++)
    {
        N_GPIO_CFG cfg = cfgs[i];
        volatile soc_reg_t func_cfg_reg_addr;
        volatile soc_reg_t dat_reg_addr;
        volatile soc_reg_t drv_reg_addr;
        volatile soc_reg_t pull_reg_addr;
        
        volatile soc_reg_t func_cfg_reg_val;
        volatile soc_reg_t drv_reg_val;
        volatile soc_reg_t pull_reg_val;
        switch (cfg.port)
        {
            case GPIO_PA :
            {
                func_cfg_reg_addr = GPIO_PA_CFG_BASE;
                dat_reg_addr = GPIO_PA_DAT_BASE;
                drv_reg_addr = GPIO_PA_DRV_BASE;
                pull_reg_addr = GPIO_PA_PULL_BASE;
                break;
            }
            case GPIO_PB :
            {
                func_cfg_reg_addr = GPIO_PB_CFG_BASE;
                dat_reg_addr = GPIO_PB_DAT_BASE;
                drv_reg_addr = GPIO_PB_DRV_BASE;
                pull_reg_addr = GPIO_PB_PULL_BASE;
                break;
            }
            case GPIO_PC :
            {
                func_cfg_reg_addr = GPIO_PC_CFG_BASE;
                dat_reg_addr = GPIO_PC_DAT_BASE;
                drv_reg_addr = GPIO_PC_DRV_BASE;
                pull_reg_addr = GPIO_PC_PULL_BASE;
                break;
            }
            case GPIO_PD :
            {
                func_cfg_reg_addr = GPIO_PD_CFG_BASE;
                dat_reg_addr = GPIO_PD_DAT_BASE;
                drv_reg_addr = GPIO_PD_DRV_BASE;
                pull_reg_addr = GPIO_PD_PULL_BASE;
                break;
            }
            case GPIO_PE :
            {
                func_cfg_reg_addr = GPIO_PE_CFG_BASE;
                dat_reg_addr = GPIO_PE_DAT_BASE;
                drv_reg_addr = GPIO_PE_DRV_BASE;
                pull_reg_addr = GPIO_PE_PULL_BASE;
                break;
            }
            case GPIO_PF :
            {
                func_cfg_reg_addr = GPIO_PF_CFG_BASE;
                dat_reg_addr = GPIO_PF_DAT_BASE;
                drv_reg_addr = GPIO_PF_DRV_BASE;
                pull_reg_addr = GPIO_PF_PULL_BASE;
                break;
            }
            case GPIO_PG :
            {
                func_cfg_reg_addr = GPIO_PB_CFG_BASE;
                dat_reg_addr = GPIO_PB_DAT_BASE;
                drv_reg_addr = GPIO_PB_DRV_BASE;
                pull_reg_addr = GPIO_PB_PULL_BASE;
                break;
            }
            default:
            {
                break;
            }
        }
        //gpio function select.
        func_cfg_reg_addr += ((cfg.port_num / 8) * 0x0004);
        func_cfg_reg_val = read32(func_cfg_reg_addr);
        func_cfg_reg_val &= ~(0x000FUL << ((cfg.port_num % 8) * 4));
        func_cfg_reg_val |= (cfg.func_select << ((cfg.port_num % 8) * 4));
        write32(func_cfg_reg_addr, func_cfg_reg_val);
        func_cfg_reg_val = read32(func_cfg_reg_addr);

        //gpio data set.
        write32(dat_reg_addr, cfg.data);

        //gpio drive level set.
        drv_reg_addr += ((cfg.port_num / 8) * 0x0004);
        drv_reg_val = read32(drv_reg_addr);
        drv_reg_val &= ~(0x000F << ((cfg.port_num % 8) * 4));
        drv_reg_val |= (cfg.drv_level << ((cfg.port_num % 8) * 4));
        write32(drv_reg_addr, drv_reg_val);

        //gpio pull state set.
        pull_reg_addr += ((cfg.port_num / 16) * 0x0004);
        pull_reg_val = read32(pull_reg_addr);
        pull_reg_val &= ~(0x003 << ((cfg.port_num % 16) * 2));
        pull_reg_val |= (cfg.pull_state << ((cfg.port_num % 16) * 2));
        write32(pull_reg_addr, pull_reg_val);
        pull_reg_val = read32(pull_reg_addr);
    }
}

void gpio_config_set(soc_reg_t cfg_reg, soc_reg_t cfg_val)
{
    *(volatile soc_reg_t *)((long)cfg_reg) = cfg_val;
}
gpio_t gpio_config_read(soc_reg_t cfg_reg)
{
    return *(volatile soc_reg_t *)((long)cfg_reg);
}
gpio_t gpio_data_read(soc_reg_t dat_reg)
{
    return *(volatile soc_reg_t *)((long)dat_reg);
}
void gpio_drv_set(soc_reg_t drv_reg, gpio_t val)
{
    *(volatile soc_reg_t *)((long)drv_reg) = val;
}
void gpio_pull_set(soc_reg_t pull_reg, gpio_t val)
{
    *(volatile soc_reg_t *)((long)pull_reg) = val;
}
void gpio_int_set(soc_reg_t eint_ctl_reg, gpio_t val)
{
    *(volatile soc_reg_t *)((long)eint_ctl_reg) = val;
}
void gpio_int_config(soc_reg_t eint_cfg_reg, gpio_t val)
{
    *(volatile soc_reg_t *)((long)eint_cfg_reg) = val;
}
