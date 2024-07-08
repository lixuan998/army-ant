#ifndef __RISCV_BASIC_OPERATION_H__
#define __RISCV_BASIC_OPERATION_H__

/****************************************************************************************************************************************************
 * @brief This file mainly defines some assembler operations embedded in C
 ****************************************************************************************************************************************************/

#include "riscv_type_defs.h"

/****************************************************************************************************************************************************
 *
 *
 * mstatus Bitmap in RV64:
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |   Bit   | Abbr Name |                       Full Name                        |                    Description                      |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    0    |    WPRI   |   Reserved Writes Preserve Values, Reads Ignore Values |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    1    |    SIE    |               Supervisor Interrupt Enable              |      Turn on/off interrupt in Supervisor mode       |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    2    |    WPRI   |   Reserved Writes Preserve Values, Reads Ignore Values |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    3    |    MIE    |               Machine Interrupt Enable                 |        Turn on/off interrupt in Machine mode        |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    4    |    WPRI   |                     Write Privilege                    |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    5    |    SPIE   |          Supervisor Previous Interrupt Enable          | Previously Turn on/off interrupt in Supervisor mode |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    6    |    UBE    |                   User Mode Big-Endian                 |             Use Big-Endian in User mode             |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    7    |    MPIE   |            Machine Previous Interrupt Enable           |   Previously Turn on/off interrupt in Machine mode  |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    8    |    SPP    |             Supervisor Previous Privilege              |             Save previous privilege mode            |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |   9~10  |    VS     |                    Virtual Supervisor                  |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |  11~12  |    MPP    |               Machine Privious Privilege               |             Save previous privilege mode            |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |  13~14  |    FS     |                  Floating-Point Status                 |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |  15~16  |    XS     |                    Extension Status                    |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    17   |    MPRV   |                    Modify Privilege                    |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    18   |    SUM    |              Supervisor User Memory Access             |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    19   |    MXR    |                 Make Excutable Readable                |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    20   |    TVM    |                  Trap Virtual Memory                   |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    21   |    TW     |                     Timeout Wait                       |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    22   |    TSR    |                        Trap SRET                       |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |  23~31  |    WPRI   |   Reserved Writes Preserve Values, Reads Ignore Values |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |  32~33  |    UXL    |                    User-mode Extension Level           |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |  34~35  |    SXL    |            Supervisr-mode Extension Level              |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    36   |    SBE    |                  Store Barrier Enable                  |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    37   |    MBE    |                Machine Breakpoint Enable               |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |  38~62  |    WPRI   |   Reserved Writes Preserve Values, Reads Ignore Values |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 * |    63   |     SD    |                      State Dirty                       |                       NONE                          |
 * +---------+-----------+--------------------------------------------------------+-----------------------------------------------------+
 *
 *
 ****************************************************************************************************************************************************/

/****************************************************************************************************************************************************
 * @brief This function's purpose is to read the Machine Status Register, which  keeps track of and controls the hart’s current
 * operating state. This register is in the Machine Mode.
 ****************************************************************************************************************************************************/
isa_reg_t inline r_mstatus()
{
    isa_reg_t mstatus;
    asm volatile("csrr %0, mstatus" : "=r"(mstatus));
    return mstatus;
}

/****************************************************************************************************************************************************
 * @brief This function's purpose is to write the Machine Status Register, which  keeps track of and controls the hart’s current
 * operating state. This register is in the Machine Mode.
 ****************************************************************************************************************************************************/
void inline w_mstatus(isa_reg_t mstatus)
{
    asm volatile("csrw mstatus, %0" : : "r"(mstatus));
}

void inline w_mepc(isa_reg_t mepc)
{
    asm volatile("csrw mepc, %0" : : "r"(mepc));
}

/****************************************************************************************************************************************************
 * @brief this function's purpose is to read the Hart ID Register, which contains the integer ID of the hardware
 * thread running the code. This register is in the Machine Mode.
 ****************************************************************************************************************************************************/
isa_reg_t inline r_mhartid()
{
    isa_reg_t mhartid;
    asm volatile("csrr %0, mhartid" : "=r"(mhartid));
    return mhartid;
}

void inline w_medeleg(isa_reg_t medeleg)
{
    asm volatile("csrw medeleg, %0" : : "r"(medeleg));
}

void inline w_mideleg(isa_reg_t mideleg)
{
    asm volatile("csrw mideleg, %0" : : "r"(mideleg));
}
/****************************************************************************************************************************************************
 * @brief this function's purpose is to read the  Supervisor Address Translation and Protection (satp) Register
 * , controls supervisor-mode address translation and protection.
 * In RV64, Mode currently has 5 options:
 * 0    -   Bare Mode   -   No translation or protection(no paging)
 * 8    -     Sv39      -   Page-based 39-bit virtual addressing
 * 9    -     Sv48      -   Page-based 48-bit virtual addressing
 * 10   -     Sv57      -   Reserved for page-based 57-bit virtual addressing
 * 11   -     Sv64      -   Reserved for page-based 64-bit virtual addressing
 * In RV32, Mode has only two options:
 * 0    -   Bare Mode   -   No translation or protection(no paging)
 * 1    -     Sv32      -   Page-based 32-bit virtual addressing
 ****************************************************************************************************************************************************/
isa_reg_t inline r_satp()
{
    isa_reg_t satp;
    asm volatile("csrr %0, satp" : "=r"(satp));
    return satp;
}

/****************************************************************************************************************************************************
 * @brief this function's purpose is to write the  Supervisor Address Translation and Protection (satp) Register
 * , controls supervisor-mode address translation and protection.
 ****************************************************************************************************************************************************/
void inline w_satp(isa_reg_t satp)
{
    asm volatile("csrw satp, %0" : : "r"(satp));
}

isa_reg_t inline r_sie()
{
    isa_reg_t sie;
    asm volatile("csrr %0, sie" : "=r"(sie));
    return sie;
}

void inline w_sie(isa_reg_t sie)
{
    asm volatile("csrw sie, %0" : : "r"(sie));
}

void inline w_pmpaddr0(isa_reg_t pmpaddr0)
{
    asm volatile("csrw pmpaddr0, %0" : : "r"(pmpaddr0));
}

void inline w_pmpcfg0(isa_reg_t pmpcfg0)
{
    asm volatile("csrw pmpcfg0, %0" : : "r"(pmpcfg0));
}

isa_reg_t inline r_tp()
{
    isa_reg_t tp;
    asm volatile("mv %0, tp" : "=r"(tp));
    return tp;
}

void inline w_tp(isa_reg_t tp)
{
    asm volatile("mv tp, %0" : : "r"(tp));
}

/****************************************************************************************************************************************************
 * @brief This function's purpose is to read the Supervisor Status Register, which  keeps track of and controls the hart’s current
 * operating state. This register is in the Machine Mode.
 ****************************************************************************************************************************************************/
isa_reg_t inline r_sstatus()
{
    isa_reg_t sstatus;
    asm volatile("csrr %0, sstatus" : "=r"(sstatus));
    return sstatus;
}

/****************************************************************************************************************************************************
 * @brief This function's purpose is to write the Supervisor Status Register, which  keeps track of and controls the hart’s current
 * operating state. This register is in the Machine Mode.
 ****************************************************************************************************************************************************/
void inline w_sstatus(isa_reg_t sstatus)
{
    asm volatile("csrw sstatus, %0" : : "r"(sstatus));
}

void inline w_stvec(isa_reg_t stvec)
{
    asm volatile("csrw stvec, %0" : : "r"(stvec));
}

isa_reg_t inline r_stvec()
{
    isa_reg_t stvec;
    asm volatile("csrr %0, stvec" : "=r"(stvec));
    return stvec;
}

isa_reg_t inline r_scause()
{
    isa_reg_t scause;
    asm volatile("csrr %0, scause" : "=r"(scause));
    return scause;
}

isa_reg_t inline r_stval()
{
    isa_reg_t stval;
    asm volatile("csrr %0, stval" : "=r"(stval));
    return stval;
}

void inline turn_on_s_interrupt()
{
    w_sstatus(r_sstatus() | SSTATUS_SIE_MASK);
}

void inline turn_off_s_interrupt()
{
    w_sstatus(r_sstatus() & ~SSTATUS_SIE_MASK);
}

int inline s_interrupt_status()
{
    return (r_sstatus() & SSTATUS_SIE_MASK);
}

void inline sfence_vma()
{
    // Flush all TLB entries.-
    asm volatile("sfence.vma zero, zero");
}

int inline get_cpu_id()
{
    return 0;
    int cpu_id = (isa_reg_t)r_tp();
    return cpu_id;
}

inline void w_sepc(uint64 x)
{
    asm volatile("csrw sepc, %0" : : "r"(x));
}

inline uint64 r_sepc()
{
    uint64 x;
    asm volatile("csrr %0, sepc" : "=r"(x));
    return x;
}

#define IOMEM(addr) ((volatile void __iomem *)((long)(addr)))

inline void write32(volatile uint32 reg, uint32 val)
{
    volatile uint32 *reg_ptr = (uint32 *)((long)reg);
    *reg_ptr = val;
}

inline uint32 read32(volatile uint32 reg)
{
    uint32 val = *((uint32 *)((long)reg));
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

extern void context_switch(CONTEXT *old_context, CONTEXT *new_context);
#endif /* __RISCV_BASIC_OPERATION_H__ */