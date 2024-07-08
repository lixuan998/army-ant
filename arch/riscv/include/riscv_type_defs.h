#ifndef __RISCV_TYPE_DEFS_H__
#define __RISCV_TYPE_DEFS_H__

#include "riscv_mem_layout.h"

typedef unsigned int                          uint;
typedef unsigned short                        ushort;
typedef unsigned char                         uchar;

typedef unsigned char                         uint8;
typedef unsigned short                        uint16;
typedef unsigned int                          uint32;
typedef unsigned long                         uint64;

typedef uint64                                isa_reg_t;
typedef uint32                                soc_reg_t;
typedef uint64                                ptr_t;

typedef uint64*                               pagetable_t;
typedef uint64                                pte_t;
typedef uint64                                addr_t;
typedef uint64                                pagesize_t;

#define NULL                                  0UL

#define bitof(x)   (sizeof(x) * 8)

/**
 * @brief CSR Related Defines.
 * 
 */
#define REGISTER_NUMBERS                      32

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
#define SSTATUS_SPP_MASK                      (1UL << SSTATUS_SPP_OFFSET)

/**
 * @brief Process Related Defines.
 * 
 */
#define VM_PROC_SHARED_MEM_ADDR (0x40000000)

enum PROC_STATE{
    PROC_STATE_RUNNING,
    PROC_STATE_READY,
    PROC_STATE_WAITING,
    PROC_STATE_EXIT,
    PROC_STATE_SLEEPING,
    PROC_STATE_ZOMBIE,
};

enum PROC_PRIORITY{
    PROC_PRIORITY_LOW,
    PROC_PRIORITY_MEDIUM,
    PROC_PRIORITY_HIGH,
};

typedef struct _TRAPFRAME{
    isa_reg_t k_satp;  // pagetable in kernel mode.
    isa_reg_t k_sp;    // used in kernel mode for processes when they trap to the kernel mode.
    isa_reg_t k_trap;
    isa_reg_t epc;     // saved user program counter.
    isa_reg_t pid;
    isa_reg_t ra;
    isa_reg_t sp;
    isa_reg_t gp;
    isa_reg_t tp;
    isa_reg_t t0;
    isa_reg_t t1;
    isa_reg_t t2;
    isa_reg_t s0;
    isa_reg_t s1;
    isa_reg_t a0;
    isa_reg_t a1;
    isa_reg_t a2;
    isa_reg_t a3;
    isa_reg_t a4;
    isa_reg_t a5;
    isa_reg_t a6;
    isa_reg_t a7;
    isa_reg_t s2;
    isa_reg_t s3;
    isa_reg_t s4;
    isa_reg_t s5;
    isa_reg_t s6;
    isa_reg_t s7;
    isa_reg_t s8;
    isa_reg_t s9;
    isa_reg_t s10;
    isa_reg_t s11;
    isa_reg_t t3;
    isa_reg_t t4;
    isa_reg_t t5;
    isa_reg_t t6;
} TRAPFRAME;

typedef struct _CONTEXT{
    isa_reg_t ra;
    isa_reg_t sp;
    isa_reg_t s0;
    isa_reg_t s1;
    isa_reg_t s2;
    isa_reg_t s3;
    isa_reg_t s4;
    isa_reg_t s5;
    isa_reg_t s6;
    isa_reg_t s7;
    isa_reg_t s8;
    isa_reg_t s9;
    isa_reg_t s10;
    isa_reg_t s11;
} CONTEXT;


typedef struct _PROC{
    void *sleep_for;
    int child_exit_ret;
    int pid;
    enum PROC_STATE proc_state;

    struct _PROC *parent;

    pagetable_t pagetable;
    addr_t phy_k_stack;
    addr_t vm_k_stack;
    addr_t shared_mem_addr;
    addr_t phy_code_addr;
    int proc_code_mem_pages;
    TRAPFRAME *trapframe;
    CONTEXT proc_context;

    enum PROC_PRIORITY priority;

    char proc_name[30];

} PROC;

typedef struct _PROC_LIST{
    PROC *procs[1000];
    int size;
    int idx;
} PROC_LIST;

/**
 * @brief CPU Related Defines.
 * 
 */
#define CPU_NUM 1
typedef struct _CPU
{
    int cpu_id;
    int slk_cnt;
    int interrupt_enabled;
    isa_reg_t reg_list[REGISTER_NUMBERS];
    CONTEXT context;
    PROC *proc;
} CPU;

/**
 * @brief Virtual Memory Related Defines.
 * 
 */
#define SV39

#define VM_ADDR_IDX(addr, idx)                   ((((isa_reg_t) (addr)) >> (idx * 9 + 12)) & 0x1FF)
#define PTE_TO_PHY_ADDR(pte)                  (((pte) >> 10) << 12)
#define PHY_ADDR_TO_PTE(phy_addr)             ((((addr_t)(phy_addr)) >> 12) << 10)
#define ADDR_TO_SATP(phy_addr)                 (((addr_t)(phy_addr)) >> 12)

#define PTE_PERMISSION_V                      (1UL << 0)
#define PTE_PERMISSION_R                      (1UL << 1)
#define PTE_PERMISSION_W                      (1UL << 2)
#define PTE_PERMISSION_X                      (1UL << 3)
#define PTE_PERMISSION_U                      (1UL << 4)
#define PTE_PERMISSION_G                      (1UL << 5)
#define PTE_PERMISSION_A                      (1UL << 6)
#define PTE_PERMISSION_D                      (1UL << 7)

#define VM_SO_STRONG_ORDER                    (1UL << 63)
#define VM_C_CACHEABLE                        (0UL << 62)
#define VM_B_BUFFERABLE                       (0UL << 61)

#define VM_MAP_SUCCESS                        (0)
#define VM_MAP_FAILED                         (-1)

#define VM_K_STACK_ADDR(pid)                  ((VM_TRAMPOLINE_ADDR - ((pid) + 1) * 2 * PAGE_SIZE))

typedef struct _VM_MAP_INFO{
    addr_t virt_addr_start;
    addr_t phys_addr_start;
    pagesize_t size;
    volatile isa_reg_t permisson;
} VM_MAP_INFO;

/**
 * @brief Memory Allocate Related Defines.
 * 
 */
#define ALIGN_FLOOR(addr)                     (((addr)) & ~(PAGE_SIZE - 1))
#define ALIGN_CEIL(addr)                      (((addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

#define RV32_SATP_MODE_OFFSET                 30
#define RV64_SATP_MODE_OFFSET                 60
#define SATP_BARE_MODE                        0UL
#define SATP_SV32_MODE                        1UL
#define SATP_SV39_MODE                        8UL
#define SATP_SV48_MODE                        9UL

#define UNALLOCTE_PAGE_VALUE                  0xFF
#define ALLOCATED_PAGE_VALUE                  0x00

typedef struct _MEM_PAGE{
    struct _MEM_PAGE *next;
} MEM_PAGE;

typedef struct _HEAP_MEM_BLK{
    int blk_free_space;
    char blk_cell[SQRT_PAGE_SIZE][SQRT_PAGE_SIZE / bitof(char)];
    struct _HEAP_MEM_BLK *next;
} HEAP_MEM_BLK;

/**
 * @brief Spinlock Related Defines.
 * 
 */
#define GENERAL_SPINLOCK_NUM        5
#define SLK_LOCKED                            1
#define SLK_UNLOCKED                          0

typedef struct SPINLOCK {
    char *slk_name;
    int locked;
    CPU *slk_owner;
} SPINLOCK;

/**
 * @brief Interrupt Related Defines.
 * 
 */
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

#endif /* __RISCV_TYPE_DEFS_H__ */