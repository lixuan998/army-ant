#include "ccu.h"

void smhcn_clk_reset(int n)
{
    volatile soc_reg_t reg_val = read32(CCU_SMHC_BGR_REG);
    reg_val |= CCU_SMHCn_BGR_RST_BIT(n);
    write32(CCU_SMHC_BGR_REG, reg_val);
}

void smhcn_clk_gating(int n, int pass)
{
    volatile soc_reg_t reg_val = read32(CCU_SMHC_BGR_REG);
    if(pass == CCU_PASS)
    {
        reg_val |= CCU_SMHCn_BGR_GATING_BIT(n);
    }
    else
    {
        reg_val &= (~CCU_SMHCn_BGR_GATING_BIT(n));
    }
    write32(CCU_SMHC_BGR_REG, reg_val);
}

void smhcn_clk_config(int n)
{
    soc_reg_t clk_reg_val = read32(CCU_SMHCn_CLK_REG(n));
    // select clock source HOSC:
    clk_reg_val |= CCU_SMHC_CLK_REG_SRC_HOSC;
    // set factor n:
    clk_reg_val |= CCU_SMHC_CLK_REG_FACTOR_N(0);
    // set factor m:
    clk_reg_val |= CCU_SMHC_CLK_REG_FACTOR_M(0);
    // turn on clock:
    clk_reg_val |= CCU_SMHC_CLK_REG_GATING_BIT;
    write32(CCU_SMHCn_CLK_REG(n), clk_reg_val);
}

void smhcn_clk_init(int n)
{
    // reset clock.
    smhcn_clk_reset(n);
    // open clock gating.
    smhcn_clk_gating(n, CCU_PASS);
    // config clock.
    smhcn_clk_config(n);
}

void ledc_clk_reset(int n)
{
    volatile soc_reg_t reg_val = read32(CCU_LEDC_BGR_REG);
    reg_val |= CCU_LEDC_BGR_REG_RST_BIT;
    write32(CCU_LEDC_BGR_REG, reg_val);
}

void ledc_clk_gating(int n, int pass)
{
    volatile soc_reg_t reg_val = read32(CCU_LEDC_BGR_REG);
    if(pass == CCU_PASS)
    {
        reg_val |= CCU_LEDC_BGR_REG_GATING_BIT;
    }
    else
    {
        reg_val &= (~CCU_LEDC_BGR_REG_GATING_BIT);
    }
    write32(CCU_LEDC_BGR_REG, reg_val);
}

void ledc_clk_config()
{
    soc_reg_t clk_reg_val = read32(CCU_LEDC_CLK_REG);
    // select clock source HOSC:
    clk_reg_val |= CCU_LEDC_CLK_REG_SRC_HOSC;
    // set factor n:
    clk_reg_val |= CCU_LEDC_CLK_REG_FACTOR_N(0);
    // set factor m:
    clk_reg_val |= CCU_LEDC_CLK_REG_FACTOR_M(0);
    // turn on clock:
    clk_reg_val |= CCU_SMHC_CLK_REG_GATING_BIT;
    write32(CCU_LEDC_CLK_REG, clk_reg_val);
}

void ledc_clk_init(int n)
{
    // reset clock.
    ledc_clk_reset(n);
    // open clock gating.
    ledc_clk_gating(n, CCU_PASS);
    // config clock.
    ledc_clk_config(n);
}