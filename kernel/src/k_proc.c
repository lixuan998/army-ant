#include "arch/defs.h"
#include "lib/include/string.h"
#include "lib/include/adt.h"
#include "bsp/driver/plic/plic.h"

SPINLOCK *pid_lock = general_spinlock;
int next_pid = 1;

LIST *proc_list = NULL;

PROC * cur_proc = NULL;

/**
 * @brief Create and allocate the basis of a process.
 * 
 * @param parent Parent process.
 * @param name Process name.
 * @param priority Process's priority.
 * @return PROC* 
 */
PROC *proc_create(PROC *parent, char *name, enum PROC_PRIORITY priority)
{
    PROC *new_proc = (PROC *)k_malloc(sizeof(PROC));

    strcpy(new_proc -> proc_name, name);
    
    //  allocate pid for new process.
    spinlock_lock(pid_lock);
    new_proc -> pid = next_pid ++;
    spinlock_unlock(pid_lock);

    new_proc -> trapframe = (TRAPFRAME *) alloc_single_page(); 
    new_proc -> parent = parent;
    new_proc -> priority = priority;

    addr_t phy_k_stack_addr = (addr_t) alloc_single_page();
    
    new_proc -> kernel_stack = (addr_t)VM_K_STACK_ADDR(new_proc -> pid);

    VM_MAP_INFO k_stack_map_info[] = {
        {
            .virt_addr_start = new_proc -> kernel_stack,
            .phys_addr_start = phy_k_stack_addr,
            .size = PAGE_SIZE,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A
        }
    };

    new_proc -> pagetable = pagetable_create(k_stack_map_info, 1);
    extern pagetable_t kernel_pagetable;
    pagetable_entry_add(kernel_pagetable, k_stack_map_info, 1);

    memset(&new_proc -> proc_context, 0, sizeof(CONTEXT));
    
    new_proc -> proc_context.sp = new_proc -> kernel_stack + PAGE_SIZE;
    new_proc -> proc_context.ra = (addr_t)proc_ret;

    new_proc -> proc_state = PROC_STATE_WAITING;

    return new_proc;
}

/**
 * @brief Prepare and add one process into process list.
 * 
 * @param proc the process wait to be added.
 * @param code the code of the process.
 * @param code_size size of the code.
 */
void exec(PROC *proc, unsigned char *code, int code_size)
{
    int map_info_entries = 3;
    int code_entry_idx = 0;
    int trampoline_entry_idx = 1;
    int trapframe_entry_idx = 2;

    VM_MAP_INFO map_info[map_info_entries];
    // Map for code
    unsigned char *code_page = (unsigned char *) alloc_single_page();
    
    memcpy((char *)code_page, (char *)code, code_size);

    map_info[code_entry_idx].virt_addr_start = (addr_t)0x00000000;
    map_info[code_entry_idx].phys_addr_start = (addr_t)code_page;
    map_info[code_entry_idx].size = PAGE_SIZE;
    map_info[code_entry_idx].permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_X | PTE_PERMISSION_D | PTE_PERMISSION_A | PTE_PERMISSION_U;

    // Map for trampoline
    map_info[trampoline_entry_idx].virt_addr_start = (addr_t)(VM_TRAMPOLINE_ADDR);
    map_info[trampoline_entry_idx].phys_addr_start = (addr_t)trampoline_start_addr;
    map_info[trampoline_entry_idx].size = PAGE_SIZE;
    map_info[trampoline_entry_idx].permisson = PTE_PERMISSION_R | PTE_PERMISSION_X | PTE_PERMISSION_A;

    // Map for trapframe
    map_info[trapframe_entry_idx].virt_addr_start = (addr_t)(VM_TRAPFRAME_ADDR);
    map_info[trapframe_entry_idx].phys_addr_start = (addr_t)proc -> trapframe;
    map_info[trapframe_entry_idx].size = PAGE_SIZE;
    map_info[trapframe_entry_idx].permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A;

    pagetable_entry_add(proc -> pagetable, map_info, 3);

    // Below are the test part, which map UART base address to user pagetable
    VM_MAP_INFO uart_map_info[] = {
        {
            .virt_addr_start = UART_BASE_ADDR,
            .phys_addr_start = UART_BASE_ADDR,
            .size = (addr_t)PAGE_SIZE,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D  | VM_SO_STRONG_ORDER | PTE_PERMISSION_A | PTE_PERMISSION_U
        }
    };

    pagetable_entry_add(proc -> pagetable, uart_map_info, 1);

    proc -> trapframe -> epc = 0x00000000;
    proc -> trapframe -> sp = PAGE_SIZE;

    proc -> proc_state = PROC_STATE_READY;
    proc_list = list_add(proc_list, proc);
}

/**
 * @brief Return address after context switch.
 * @note This function is set as the value of ra register before context switch.
 */
void proc_ret()
{
    printf("PROC RET\n\r");
    // Trap to user space.
    kernel_trap();
    printf("kernel_trap RET\n\r");
}

/**
 * @brief Trap from kernel space to user space.
 */
void kernel_trap()
{
    PROC *cur_proc = current_cpu_proc();

    interrupt_disable();
    
    extern char user_intr_interface[];
    addr_t vm_user_intr_interface_addr = (addr_t)VM_TRAMPOLINE_ADDR + ((addr_t)user_intr_interface - (addr_t)trampoline);
    w_stvec(vm_user_intr_interface_addr);
    // Set up trapframe values
    cur_proc -> trapframe -> k_satp = r_satp();
    cur_proc -> trapframe -> k_sp = cur_proc -> kernel_stack + PAGE_SIZE;
    cur_proc -> trapframe -> k_trap = (addr_t)user_trap;
    cur_proc -> trapframe -> pid = cur_proc -> pid;

    isa_reg_t sstatus_val = r_sstatus();

    // Make sret return to user mode
    sstatus_val &= ~SSTATUS_SPP_MASK;
    // Enable interrupts in user mode
    sstatus_val |= SSTATUS_SPIE_MASK;

    w_sstatus(sstatus_val);

    // Set up epc, which will be the entrance of user programe later
    w_sepc((cur_proc -> trapframe -> epc));
    addr_t user_satp = ((SATP_SV39_MODE << RV64_SATP_MODE_OFFSET) | ADDR_TO_SATP((addr_t)cur_proc -> pagetable));

    addr_t vm_ret_to_user_func_addr = (addr_t)VM_TRAMPOLINE_ADDR + (addr_t)((addr_t)ret_to_user - (addr_t)trampoline);
    ((void (*)(addr_t))vm_ret_to_user_func_addr)(user_satp);
}

/**
 * @brief Trap from user space to kernel space.
 * @note Called from user_intr_interface in trampoline.S.
 */
void user_trap()
{
    if((r_scause() & 0xFF) == 9)
    {
        interrupt_handler();
    }
    
    else
    printf("stval: %x\n\r", r_stval());
    if((r_sstatus() & SSTATUS_SPP_MASK) != 0)
    panic("usertrap: not from user mode");
    w_stvec((isa_reg_t)kernel_interrupt_vector);
    PROC *cur_proc = current_cpu_proc();
    cur_proc->trapframe->epc = r_sepc();
    kernel_trap();
}

void init_proc()
{
    PROC *init = proc_create(NULL, "init", PROC_PRIORITY_HIGH);
    // unsigned char initcode[] = {
    //     0x17, 0x05, 0x00, 0x00, 0x13, 0x05, 0x45, 0x02,
    //     0x97, 0x05, 0x00, 0x00, 0x93, 0x85, 0x35, 0x02,
    //     0x93, 0x08, 0x70, 0x00, 0x73, 0x00, 0x00, 0x00,
    //     0x93, 0x08, 0x20, 0x00, 0x73, 0x00, 0x00, 0x00,
    //     0xef, 0xf0, 0x9f, 0xff, 0x2f, 0x69, 0x6e, 0x69,
    //     0x74, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00,
    //     0x00, 0x00, 0x00, 0x00
    //     };
    // unsigned char code[] = {0xb7, 0x07, 0x50, 0x02, 0x13, 0x07, 0x80, 0x04, 0x23, 0xa0, 0xe7, 0x00, 0x13, 0x05, 0x00, 0x00, 0x67, 0x80, 0x00, 0x00};
    unsigned char code[] = {0xb7, 0x07, 0x50, 0x02, 0x13, 0x07, 0x30, 0x04, 0x23, 0xa0, 0xe7, 0x00, 0x67, 0x80, 0x00, 0x00};
    // unsigned char code[] = {0x6F, 0x00, 0x00, 0x00};
    printf("sizeof code: %d\n\r", sizeof(code));
    exec(init, code, sizeof(code));
}

void scheduler()
{
    cur_proc = (PROC *)proc_list -> data;
    for(;;)
    {
        printf("cur proc : %x\n\r", cur_proc);
        while(cur_proc)
        {            
            if(cur_proc -> proc_state == PROC_STATE_READY)
            {
                CPU *cpu = current_cpu();
                cpu->proc = cur_proc;
                printf("func addr: %x, ra addr: %x\n\r", (addr_t)proc_ret, cur_proc -> proc_context.ra);
                printf("scheduling : %s\n\r", cur_proc -> proc_name);
                printf("READY\n\r");
                cur_proc -> proc_state = PROC_STATE_RUNNING;
                context_switch(&current_cpu() -> context, &cur_proc -> proc_context);
                printf("AAAa0: %d\n", cur_proc -> trapframe -> a0);
                cur_proc -> proc_state = PROC_STATE_READY;
            }
            // sleep(1);
        }
    }
    
}

void back_to_scheduler()
{
    PROC *p = current_cpu_proc();
    printf("cur proc name: %s\n\r", p -> proc_name);
    printf("last_context ra: %x\n\r", current_cpu() -> context.ra);
    w_sepc(p->trapframe->epc);
    w_satp((isa_reg_t)p ->pagetable);
    asm volatile("sret");
    context_switch(&cur_proc -> proc_context, &current_cpu() -> context);
}