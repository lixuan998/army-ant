#ifndef __CCU_H__
#define __CCU_H__

#include "arch/riscv/include/riscv_type_defs.h"
#include "arch/riscv/include/riscv_basic_operation.h"

#define CCU_BASE_ADDR                   (0x02001000UL)

#define CCU_PLL_CPU_CTRL_REG            (CCU_BASE_ADDR + 0x0000UL)
#define CCU_PLL_DDR_CTRL_REG            (CCU_BASE_ADDR + 0x0010UL)
#define CCU_PLL_PERI_CTRL_REG           (CCU_BASE_ADDR + 0x0020UL)

// SMHC related registers.
#define CCU_SMHCn_CLK_REG(n)            (CCU_BASE_ADDR + (0x0830UL + n * 4))
#define CCU_SMHC_BGR_REG                (CCU_BASE_ADDR + 0x084CUL)

// CCU_SMHC_BGR_REG bits.
#define CCU_SMHCn_BGR_GATING_BIT(n)     (1UL << n)
#define CCU_SMHCn_BGR_RST_BIT(n)        (1UL << (16 + n))

// SMHCn_CLK_REG bits.
#define CCU_SMHC_CLK_REG_GATING_BIT     (1UL << 31)
#define CCU_SMHC_CLK_REG_SRC_HOSC       (0UL << 24)
#define CCU_SMHC_CLK_REG_SRC_PLL_PERI1x (1UL << 24)
#define CCU_SMHC_CLK_REG_SRC_PLL_PERI2x (2UL << 24)
#define CCU_SMHC_CLK_REG_FACTOR_N(val)  (val << 8)
#define CCU_SMHC_CLK_REG_FACTOR_M(val)  (val << 0)

// LEDC related registers.
#define CCU_LEDC_CLK_REG                (CCU_BASE_ADDR + 0x0BF0UL)
#define CCU_LEDC_BGR_REG                (CCU_BASE_ADDR + 0x0BFCUL)

// LEDC_CLK_REG bits.
#define CCU_LEDC_CLK_REG_GATING_BIT     (1UL << 31)
#define CCU_LEDC_CLK_REG_SRC_HOSC       (0UL << 24)
#define CCU_LEDC_CLK_REG_SRC_PLL_PERI1x (1UL << 24)
#define CCU_LEDC_CLK_REG_SRC_PLL_PERI2x (2UL << 24)
#define CCU_LEDC_CLK_REG_FACTOR_N(val)  (val << 8)
#define CCU_LEDC_CLK_REG_FACTOR_M(val)  (val << 0)

// LEDC_CLK_REG bits.
#define CCU_LEDC_BGR_REG_GATING_BIT     (1UL)
#define CCU_LEDC_BGR_REG_RST_BIT        (1UL << 16)

#define CCU_PASS                        (1)
#define CCU_MASK                        (0)

void smhcn_clk_reset(int n);
void smhcn_clk_gating(int n, int pass);
void smhcn_clk_config(int n);
void smhcn_clk_init(int n);

void ledc_clk_reset(int n);
void ledc_clk_gating(int n, int pass);
void ledc_clk_config();

#endif  /* __CCU_H__ */