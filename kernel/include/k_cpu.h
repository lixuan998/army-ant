#ifndef __K_CPU__
#define __K_CPU__

#include "k_defs.h"
#include "../../arch/riscv/include/asm_operation.h"

#define MAX_CPU_NUM 8
#define REGS_NUM    32
#define REG_RA      0
#define REG_SP      1
#define REG_FP      2
#define REG_SX      2

typedef struct _K_CPU{
    int cpu_id;
    int slk_cnt;
    int intrrupt_enabled;
    uint64 reg_list[REGS_NUM];

} K_CPU;

int k_get_cpu_id();

#endif /* __K_CPU__ */