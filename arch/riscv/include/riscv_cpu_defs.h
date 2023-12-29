#ifndef __RISCV_CPU_DEFS_H__
#define __RISCV_CPU_DEFS_H__

#include "riscv_type_defs.h"
#include "riscv_csr_defs.h"
#include "riscv_asm_operation.h"
// #include "riscv_proc_defs.h"

#define CPU_NUM                               1

typedef struct _CPU{
    int cpu_id;
    int slk_cnt;
    int interrupt_enabled;
    isa_reg_t reg_list[REGISTER_NUMBERS];
    // CONTEXT context;
    // PROC *  proc;
} CPU;

CPU * current_cpu();
// PROC * current_cpu_proc();

#endif  /* __RISCV_CPU_DEFS_H__ */