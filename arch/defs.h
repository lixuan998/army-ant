#ifndef __DEFS_H_
#define __DEFS_H_

#include "arch/riscv/include/riscv_defs.h"

inline void write_reg32(volatile uint32 reg, uint32 val)
{
    volatile uint32 *reg_ptr = (uint32 *)((long)reg);
    *reg_ptr = val;
}

inline int read_reg32(volatile uint32 reg)
{
    volatile int val = *((uint32 *)((long)reg));
    return val;
}

#endif /* __DEFS_H__ */