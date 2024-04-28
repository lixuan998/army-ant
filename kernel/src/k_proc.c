#include "arch/defs.h"
#include "lib/include/string.h"
#include "lib/include/adt.h"

SPINLOCK *pid_lock = general_spinlock;
int next_pid = 1;

LIST *proc_list = NULL;

CONTEXT last_context;
PROC * cur_proc = NULL;

PROC *proc_create(PROC *parent, char *name, enum PROC_PRIORITY priority)
{
    PROC *new_proc = (PROC *)malloc(sizeof(PROC));
    spinlock_init(&(new_proc -> lock), "proc_spinlock");
    spinlock_lock(&(new_proc -> lock));

    strcpy(new_proc -> proc_name, name);
    
    //  allocate pid for new process.
    spinlock_lock(pid_lock);
    new_proc -> pid = next_pid ++;
    spinlock_unlock(pid_lock);

    new_proc -> system_regs = (SYS_REGS *) alloc_single_page(); 
    new_proc -> user_stack = (addr_t) alloc_single_page();
    new_proc -> kernel_stack = (addr_t) alloc_single_page();
    new_proc -> parent = parent;
    new_proc -> priority = priority;

    memset(&new_proc -> proc_context, 0, sizeof(CONTEXT));
    new_proc -> proc_context.sp = new_proc -> kernel_stack + PAGE_SIZE;
    new_proc -> proc_context.ra = (addr_t)proc_ret;

    new_proc -> proc_state = PROC_STATE_WAITING;

    return new_proc;
}

void exec(PROC *proc, char *code, int code_size)
{
    int page_num = (code_size / PAGE_SIZE + 1);
    proc -> proc_mem_size = PAGE_SIZE;
    for(int i = 0; i < page_num; ++ i)
    {
        proc -> pages[i] = (MEM_PAGE *)alloc_single_page();
    }
    proc -> mem_blk_head = (MEM_BLK *)alloc_single_page();
    blk_init(proc -> mem_blk_head);
    
    for(int i = 0; i < page_num; ++ i)
    {
        memcpy(proc -> pages[i], code, PAGE_SIZE);
        code += PAGE_SIZE;
    }

    VM_MAP_INFO map_info[page_num + 1];
    for(int i = 0; i < page_num; ++ i)
    {
        map_info[i].virt_addr_start = (addr_t)(i * PAGE_SIZE);
        map_info[i].phys_addr_start = (addr_t)proc -> pages[i];
        map_info[i].size = PAGE_SIZE;
        map_info[i].permisson = PTE_PERMISSION_R | PTE_PERMISSION_X | PTE_PERMISSION_A | VM_C_CACHEABLE | VM_B_BUFFERABLE;
    }

    map_info[page_num].virt_addr_start = (addr_t)(PAGE_SIZE);
    map_info[page_num].phys_addr_start = (addr_t)(proc -> mem_blk_head);
    map_info[page_num].size = PAGE_SIZE;
    map_info[page_num].permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A | VM_C_CACHEABLE | VM_B_BUFFERABLE;

    map_info[page_num + 1].virt_addr_start = (addr_t)(page_num * PAGE_SIZE);
    map_info[page_num + 1].phys_addr_start = (addr_t)proc -> user_stack;
    map_info[page_num + 1].size = PAGE_SIZE;
    map_info[page_num + 1].permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A | VM_C_CACHEABLE | VM_B_BUFFERABLE;

    printf("before pagetable create\n\r");
    proc -> pagetable = pagetable_create(map_info, page_num + 1);
    printf("after pagetable create\n\r");
    proc -> system_regs -> epc = 0;
    proc -> system_regs -> sp = proc -> user_stack;

    proc -> proc_state = PROC_STATE_READY;
    proc_list = list_add(proc_list, proc);
}

void proc_ret()
{
    printf("PROC RET\n\r");
    printf("BBBa0: %d\n\r", cur_proc -> system_regs -> a0);
    back_to_scheduler();
}

void init_proc()
{
    PROC *init = proc_create(NULL, "init", PROC_PRIORITY_HIGH);
    char initcode[] = {
        0x17, 0x05, 0x00, 0x00, 0x13, 0x05, 0x45, 0x02,
        0x97, 0x05, 0x00, 0x00, 0x93, 0x85, 0x35, 0x02,
        0x93, 0x08, 0x70, 0x00, 0x73, 0x00, 0x00, 0x00,
        0x93, 0x08, 0x20, 0x00, 0x73, 0x00, 0x00, 0x00,
        0xef, 0xf0, 0x9f, 0xff, 0x2f, 0x69, 0x6e, 0x69,
        0x74, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00
        };
    char code[] = {0xef, 0x00, 0x00, 0x00};
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
                printf("scheduling : %s\n\r", cur_proc -> proc_name);
                printf("READY\n\r");
                cur_proc -> proc_state = PROC_STATE_RUNNING;
                context_switch(&last_context, &cur_proc -> proc_context);
                printf("AAAa0: %d\n", cur_proc -> system_regs -> a0);
                cur_proc -> proc_state = PROC_STATE_READY;
            }
            // sleep(1);
        }
    }
    
}

void back_to_scheduler()
{
    printf("last_context ra: %x\n\r", last_context.ra);
    context_switch(&cur_proc -> proc_context, &last_context);
}