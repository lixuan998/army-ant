#ifndef __RISCV_DEFS_H_
#define __RISCV_DEFS_H_

#define RV64
#define SV39

/****************************************************************************************************************************************************
 * Types related definitions.
****************************************************************************************************************************************************/
#ifdef RV64
typedef unsigned int                          uint;
typedef unsigned short                        ushort;
typedef unsigned char                         uchar;

typedef unsigned char                         uint8;
typedef unsigned short                        uint16;
typedef unsigned int                          uint32;
typedef unsigned long                         uint64;

typedef uint64*                               pagetable_t;
typedef uint64                                pte_t;
typedef uint64                                addr_t;
typedef uint64                                pagesize_t;

#endif /* RV64 */

#define NULL                                  0

/****************************************************************************************************************************************************
 * RISC-V related definitions.
****************************************************************************************************************************************************/
#define USER_MODE_CODE                        0
#define SUPERVISOR_MODE_CODE                  1
#define HYPERVISOR_MODE_CODE                  2
#define MACHINE_MODE_CODE                     3

#define MSTATUS_SIE_OFFSET                    1
#define MSTATUS_SIE_MASK                      1UL << MSTATUS_SIE_OFFSET
#define MSTATUS_MIE_OFFSET                    3
#define MSTATUS_MIE_MASK                      1UL << MSTATUS_MIE_OFFSET
#define MSTATUS_SPIE_OFFSET                   5
#define MSTATUS_SPIE_MASK                     1UL << MSTATUS_SPIE_OFFSET
#define MSTATUS_MPIE_OFFSET                   7
#define MSTATUS_MPIE_MASK                     1UL << MSTATUS_MPIE_OFFSET
#define MSTATUS_SPP_OFFSET                    8
#define MSTATUS_SPP_MASK                      1UL << MSTATUS_SPP_OFFSET
#define MSTATUS_MPP_OFFSET                    11UL
#define MSTATUS_MPP_MASK                      3UL << MSTATUS_MPP_OFFSET

#define SSTATUS_SIE_OFFSET                    1
#define SSTATUS_SIE_MASK                      1UL << SSTATUS_SIE_OFFSET
#define SSTATUS_SPIE_OFFSET                   5
#define SSTATUS_SPIE_MASK                     1UL << SSTATUS_SPIE_OFFSET
#define SSTATUS_SPP_OFFSET                    8
#define SSTATUS_SPP_MASK                      1UL << SSTATUS_SPP_OFFSET

/****************************************************************************************************************************************************
 * CPU related definitions.
****************************************************************************************************************************************************/
#define CPU_NUM                               1
#define REGS_NUM                              32
#define REG_RA                                0
#define REG_SP                                1
#define REG_FP                                2
#define REG_SX                                2

typedef struct _CPU{
    int cpu_id;
    int slk_cnt;
    int interrupt_enabled;
    uint64 reg_list[REGS_NUM];

} CPU;

/****************************************************************************************************************************************************
 * Spinlock related definitions.
****************************************************************************************************************************************************/
#define SLK_LOCKED                            1
#define SLK_UNLOCKED                          0

typedef struct SPINLOCK {
    char *slk_name;
    int locked;
    CPU *slk_owner;
} SPINLOCK;

/****************************************************************************************************************************************************
 * Interrupt & Exception related definitions.
****************************************************************************************************************************************************/
#define SIE_SSIE_OFFSET                       1
#define SIE_SSIE_MASK                         1UL << SIE_SSIE_OFFSET
#define SIE_STIE_OFFSET                       5
#define SIE_STIE_MASK                         1UL << SIE_STIE_OFFSET
#define SIE_SEIE_OFFSET                       9
#define SIE_SEIE_MASK                         1 << SIE_SEIE_OFFSET

#define SCAUSE_INTERRUPT                      1UL << 63
#define SCAUSE_EXCEPTION                      0UL << 63
#define SCAUSE_EXCEPTION_CODE_MASK            0x7FFFFFFFFFFFFFFFUL
#define SCAUSE_SOFTWARE_INTERRUPT             1
#define SCAUSE_TIMER_INTERRUPT                5
#define SCAUSE_EXTERNAL_INTERRUPT             9

#define SCAUSE_INST_ADDR_MISALIGN             0
#define SCAUSE_INST_ACCESS_FAULT              1
#define SCAUSE_ILLEGAL_INST                   2
#define SCAUSE_BREAKPOINT                     3
#define SCAUSE_LOAD_ADDR_MISALIGN             4
#define SCAUSE_LOAD_ACCESS_FAULT              5
#define SCAUSE_STORE_AMO_ADDR_MISALIGN        6
#define SCAUSE_STORE_AMO_ACCESS_FAULT         7
#define SCAUSE_ECALL_U                        8
#define SCAUSE_ECALL_S                        9
#define SCAUSE_INST_PAGE_FAULT                12
#define SCAUSE_LOAD_PAGE_FAULT                13
#define SCAUSE_STORE_AMO_PAGE_FAULT           15

/****************************************************************************************************************************************************
 * Memory Paging related definitions.
****************************************************************************************************************************************************/
#define PAGE_SIZE                             (4096)
#define ALIGN_FLOOR(addr)                     (((addr)) & ~(PAGE_SIZE - 1))
#define ALIGN_CEIL(addr)                      (((addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

#define RV32_SATP_MODE_OFFSET                 30
#define RV64_SATP_MODE_OFFSET                 60
#define SATP_BARE_MODE                        0UL
#define SATP_SV32_MODE                        1UL
#define SATP_SV39_MODE                        8UL
#define SATP_SV48_MODE                        9UL

// Provided in linkld.ld
extern addr_t                                 kernel_start[];
extern addr_t                                 kernel_end[];
extern addr_t                                 kernel_text_end[];

#define UNALLOCTE_PAGE_VALUE                  0xFF
#define ALLOCATED_PAGE_VALUE                  0x00

typedef struct _MEM_PAGE{
    struct _MEM_PAGE *next;
} MEM_PAGE;

/****************************************************************************************************************************************************
 * Virtual Memory Mapping related definitions.
****************************************************************************************************************************************************/
#define RAM_SIZE                              (60 * 1024 * 1024)
#define RAM_TOP                               (addr_t)kernel_start + (RAM_SIZE - 1UL)
#define ADDR_MAX_VAL                          (1UL << (9 + 9 + 9 + 12))
#define ADDR_IDX(addr, idx)                   ((((uint64) (addr)) >> (idx * 9 + 12)) & 0x1FF)
#define PTE_TO_PHY_ADDR(pte)                  (((pte) >> 10) << 12)
#define PHY_ADDR_TO_PTE(phy_addr)             ((((addr_t)(phy_addr)) >> 12) << 10)
#define PHY_TO_SATP(phy_addr)                 (((addr_t)(phy_addr)) >> 12)

#define PTE_PERMISSION_V                      (1UL << 0)
#define PTE_PERMISSION_R                      (1UL << 1)
#define PTE_PERMISSION_W                      (1UL << 2)
#define PTE_PERMISSION_X                      (1UL << 3)
#define PTE_PERMISSION_U                      (1UL << 4)
#define PTE_PERMISSION_G                      (1UL << 5)
#define PTE_PERMISSION_A                      (1UL << 6)
#define PTE_PERMISSION_D                      (1UL << 7)

#define VM_SO_STRONG_ORDER                    (1UL << 63)
#define VM_C_CACHEABLE                        (1UL << 62)
#define VM_B_BUFFERABLE                       (1UL << 61)

#define VM_MAP_SUCCESS                        (0)
#define VM_MAP_FAILED                         (-1)



#endif /* __RISCV_DEFS_H__ */