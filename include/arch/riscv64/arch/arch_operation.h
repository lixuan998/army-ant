#pragma once

#include <lib/stdint.h>
#include <arch/arch_defs.h>

#define READ_CSR(csr)							        \
        ({uintptr_t value;						        \
        asm volatile("csrr %0, " #csr : "=r" (value));				\
        value;})

#define WRITE_CSR(csr, value)							\
        do {								        \
            asm volatile("csrw " #csr ", %0" : : "r" ((uintptr_t)(value)));	\
        } while (0)

#define WRITE_GPR(reg, value)                                                   \
        do {                                                                    \
            asm volatile("mv " #reg ", %0" : : "r" ((uintptr_t)(value)));       \
        } while (0)

#define READ_GPR(reg)                                                           \
        ({uintptr_t value;                                                      \
        asm volatile("mv %0, " #reg : "=r" (value));                            \
        value;})
#define READ_HARTID()                                                           \
        READ_GPR(tp)

#define INVALIDATE_TLB()                                                        \
        asm volatile("sfence.vma zero, zero")

#define INVALIDATE_TLB_VA(va)                                                   \
        asm volatile("sfence.vma %0, zero" : : "r" (va))

#define INVALIDATE_TLB_ASID(asid)                                               \
        asm volatile("sfence.vma zero, %0" : : "r" (asid))

#define INVALIDATE_TLB_VA_ASID(va, asid)                                        \
        asm volatile("sfence.vma %0, %1" : : "r" (va), "r" (asid))
