#ifndef __RISCV_CPU_H__
#define __RISCV_CPU_H__

#include "arch/defs.h"
#include "arch/riscv/include/riscv_asm_operation.h"

#define MAX_CPU_NUM 8
#define REGS_NUM    32
#define REG_RA      0
#define REG_SP      1
#define REG_FP      2
#define REG_SX      2

typedef struct _CPU{
    int cpu_id;
    int slk_cnt;
    int intrrupt_enabled;
    uint64 reg_list[REGS_NUM];

} CPU;

int get_cpu_id();

#endif /* __RISCV_CPU_H__ */