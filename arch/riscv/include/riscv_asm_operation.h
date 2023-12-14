#ifndef __ASM_OPERATION_H__
#define __ASM_OPERATION_H__

/****************************************************************************************************************************************************
 * @brief This file mainly defines some assembler operations embedded in C
****************************************************************************************************************************************************/

#include "kernel/include/k_defs.h"
#include "arch/defs.h"

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
uint64 inline r_mstatus()
{
  uint64 mstatus;
  asm volatile("csrr %0, mstatus" : "=r" (mstatus));
  return mstatus;
}

/****************************************************************************************************************************************************
 * @brief This function's purpose is to write the Machine Status Register, which  keeps track of and controls the hart’s current
 * operating state. This register is in the Machine Mode.
****************************************************************************************************************************************************/
void inline w_mstatus(uint64 mstatus)
{
  asm volatile("csrw mstatus, %0" : : "r" (mstatus));
}

void inline w_mepc(uint64 mepc)
{
    asm volatile ("csrw mepc, %0" : : "r" (mepc));
}

/****************************************************************************************************************************************************
 * @brief this function's purpose is to read the Hart ID Register, which contains the integer ID of the hardware
 * thread running the code. This register is in the Machine Mode.
****************************************************************************************************************************************************/
uint64 inline r_mhartid()
{
  uint64 mhartid;
  asm volatile("csrr %0, mhartid" : "=r" (mhartid));
  return mhartid;
}

void inline w_medeleg(uint64 medeleg)
{
  asm volatile("csrw medeleg, %0" : : "r" (medeleg));
}

void inline w_mideleg(uint64 mideleg)
{
  asm volatile("csrw mideleg, %0" : : "r" (mideleg));
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
uint64 inline r_satp()
{
  uint64 satp;
  asm volatile("csrr %0, satp" : "=r" (satp));
  return satp;
}

/****************************************************************************************************************************************************
 * @brief this function's purpose is to write the  Supervisor Address Translation and Protection (satp) Register
 * , controls supervisor-mode address translation and protection.
****************************************************************************************************************************************************/
void inline w_satp(uint64 satp)
{
  asm volatile("csrw satp, %0" : : "r" (satp));
}

uint64 inline r_sie()
{
  uint64 sie;
  asm volatile("csrr %0, sie" : "=r" (sie));
  return sie;
}

void inline w_sie(uint64 sie)
{
  asm volatile("csrw sie, %0" : : "r" (sie));
}


void inline w_pmpaddr0(uint64 x)
{
  asm volatile("csrw pmpaddr0, %0" : : "r" (x));
}

void inline w_pmpcfg0(uint64 x)
{
  asm volatile("csrw pmpcfg0, %0" : : "r" (x));
}

uint64 inline r_tp()
{
  uint64 tp;
  asm volatile("mv %0, tp" : "=r" (tp));
  return tp;
}

void inline w_tp(uint64 tp)
{
  asm volatile("mv tp, %0" : : "r" (tp));
}

/****************************************************************************************************************************************************
 * @brief This function's purpose is to read the Supervisor Status Register, which  keeps track of and controls the hart’s current
 * operating state. This register is in the Machine Mode.
****************************************************************************************************************************************************/
uint64 inline r_sstatus()
{
  uint64 sstatus;
  asm volatile("csrr %0, sstatus" : "=r" (sstatus));
  return sstatus;
}

/****************************************************************************************************************************************************
 * @brief This function's purpose is to write the Supervisor Status Register, which  keeps track of and controls the hart’s current
 * operating state. This register is in the Machine Mode.
****************************************************************************************************************************************************/
void inline w_sstatus(uint64 sstatus)
{
  asm volatile("csrw sstatus, %0" : : "r" (sstatus));
}

void inline w_stvec(uint64 stvec)
{
  asm volatile("csrw stvec, %0" : : "r" (stvec));
}

uint64 inline r_stvec()
{
  uint64 stvec;
  asm volatile("csrr %0, stvec" : "=r" (stvec) );
  return stvec;
}

uint64 inline r_scause()
{
  uint64 scause;
  asm volatile("csrr %0, scause" : "=r" (scause) );
  return scause;
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
#endif  /* __ASM_OPERATION_H__ */