#ifndef __DEFS_H_
#define __DEFS_H_

#include "arch/riscv/include/riscv_defs.h"

#define IOMEM(addr) ((volatile void __iomem *)((long)(addr)))

inline void write32(volatile uint32 reg, uint32 val)
{
    volatile uint32 *reg_ptr = (uint32 *)((long)reg);
    *reg_ptr = val;
}

inline int read32(volatile uint32 reg)
{
    volatile int val = *((uint32 *)((long)reg));
    return val;
}

inline void write64(volatile uint64 reg, uint64 val)
{
    volatile uint64 *reg_ptr = (uint64 *)((long)reg);
    *reg_ptr = val;
}

inline uint64 read64(volatile uint64 reg)
{
    volatile uint64 val = *((uint64 *)((long)reg));
    return val;
}

#endif /* __DEFS_H__ */