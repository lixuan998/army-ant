#include "init.h"

/****************************************************************************************************************************************************
 * Spinlock related functions.
****************************************************************************************************************************************************/

void spinlock_init(SPINLOCK *slk, char *name)
{
    // slk -> slk_name = name;
    // slk -> locked = SLK_UNLOCKED;
    // slk -> slk_owner = NULL;
}

void spinlock_lock(SPINLOCK *slk)
{
    // int interrupt_status = s_interrupt_status();

    // CPU *cur_cpu = current_cpu();
    // cur_cpu->slk_cnt ++;
    // cur_cpu->interrupt_enabled = interrupt_status;
    
    // if(slk -> locked && slk -> slk_owner == cur_cpu)
    // {
    //     panic(__FILE__, __LINE__, "riscv_spinlock_lock");
    // }

    // while(__sync_lock_test_and_set(&slk->locked, 1) != 0);
    // __sync_synchronize();

    // slk -> slk_owner = cur_cpu;

    // interrupt_disable();
}

void spinlock_unlock(SPINLOCK *slk)
{
    // CPU *cur_cpu = current_cpu();
    // if(!(slk -> locked && slk -> slk_owner == cur_cpu))
    // {
    //     panic(__FILE__, __LINE__, "riscv_spinlock_unlock");
    // }

    // slk -> slk_owner = NULL;
    // __sync_synchronize();
    // __sync_lock_release(&slk->locked);

    // cur_cpu -> slk_cnt --;
    // if(cur_cpu -> slk_cnt == 0 && cur_cpu -> interrupt_enabled)
    // {
    //     interrupt_enable();
    // }
}

/****************************************************************************************************************************************************
 * Memory Paging related functions.
****************************************************************************************************************************************************/
SPINLOCK mem_paging_spinlock;   // Initialized in kernel/src/k_paging.c

MEM_PAGE *mem_page_list = NULL;

void *alloc_single_page()
{
    if(mem_page_list == NULL)
    {
        panic(__FILE__, __LINE__, "in k_alloc_single_page, no memory page left");
    }
    spinlock_lock(&mem_paging_spinlock);
    MEM_PAGE *page = mem_page_list;
    mem_page_list = mem_page_list -> next;
    spinlock_unlock(&mem_paging_spinlock);

    memset(page, ALLOCATED_PAGE_VALUE, PAGE_SIZE);
    return (void *)page;
}

void free_single_page(void *page)
{
    if((addr_t)page % PAGE_SIZE || (addr_t *)page < kernel_end || (addr_t)page > RAM_TOP)
    {
        panic(__FILE__, __LINE__, "in k_free_single_page, page out of bound");
    }
    memset(page, UNALLOCTE_PAGE_VALUE, PAGE_SIZE);

    spinlock_lock(&mem_paging_spinlock);
    MEM_PAGE *new_page = (MEM_PAGE *)page;
    new_page->next = mem_page_list;
    mem_page_list = new_page;
    spinlock_unlock(&mem_paging_spinlock);
}

/****************************************************************************************************************************************************
 * Interrupt related functions.
****************************************************************************************************************************************************/
void interrupt_init()
{
    extern void k_interrupt_vector();
    w_stvec((isa_reg_t)k_interrupt_vector);
    plic_interrupt_enable();
    interrupt_enable();
}

void interrupt_enable()
{
    turn_on_s_interrupt();
}

void interrupt_disable()
{
    turn_off_s_interrupt();
}

void interrupt_handler()
{
    isa_reg_t scause = r_scause();
    isa_reg_t scause_code = r_scause() & SCAUSE_EXCEPTION_CODE_MASK;
    if(scause & SCAUSE_INTERRUPT)
    {
        switch(scause_code)
        {
            case SCAUSE_SOFTWARE_INTERRUPT:
            {
                software_interrupt_handler();
                break;
            }
            case SCAUSE_TIMER_INTERRUPT:
            {
                timer_interrupt_handler();
                break;
            } 
            case SCAUSE_EXTERNAL_INTERRUPT:
            {
                external_interrupt_handler();
                break;
            }
            default:
                break;
        }
        
    }
}

void external_interrupt_handler()
{
    enum EXTERNAL_INTERRUPT_SOURCE source;
    source = read_reg32(PLIC_SCLAIM_REG);
    switch (source)
    {
        case UART0_SOURCE:
        {
            // printf("uart0 interrupt\n\r");
            char c;
            int ret = getc(&c);
            while(ret >= 0)
            {
                console_get_char(c);
                ret = getc(&c);
            }
            break;
        }
        case TIMER0_SOURCE:
        {
            timestamp_update();
            timer_pending_clear(0);
            break;
        }
        
        default:
            break;
        }
    external_interrupt_handled(source);
}

void external_interrupt_handled(int source)
{
    write_reg32(PLIC_SCLAIM_REG, source);
}

void timer_interrupt_handler()
{

}

void software_interrupt_handler()
{

}

/****************************************************************************************************************************************************
 * Virtual Memory related functions.
****************************************************************************************************************************************************/
int vm_map(pagetable_t pagetable, addr_t virt_addr_start, addr_t phys_addr_start, pagesize_t size, isa_reg_t permisson)
{
    if(size == 0)
    {
        panic(__FILE__, __LINE__, "in vm_map, page size invalid.");
    }

    pte_t *pte;
    virt_addr_start = ALIGN_FLOOR(virt_addr_start);
    addr_t virt_addr_end = ALIGN_FLOOR(virt_addr_start + size);

    //It's not necessary to align phys_addr_start, it's only for making the whole process more explicit.printf("phy addr: %x\n\r", phys_addr_start);
    phys_addr_start = ALIGN_FLOOR(phys_addr_start);

    while(virt_addr_start < virt_addr_end)
    {
        pte = pte_retrieve(pagetable, virt_addr_start);
        if(pte == NULL)
        {
            printf("in vm_map, pte is NULL.");
            return VM_MAP_FAILED;
        }
        if((*pte) & PTE_PERMISSION_V)
        {
            panic(__FILE__, __LINE__, "in vm_map, pte already mapped.");
        }
        (*pte) = PHY_ADDR_TO_PTE(phys_addr_start) | permisson | PTE_PERMISSION_V;
        phys_addr_start += PAGE_SIZE;
        virt_addr_start += PAGE_SIZE;
    }
    return VM_MAP_SUCCESS;
}

pte_t* pte_retrieve(pagetable_t pagetable, addr_t virt_addr)
{
    if(virt_addr >= ADDR_MAX_VAL)
    {
        panic(__FILE__, __LINE__, "in pte_retrieve, virt_addr exceeded.");
    }
    for (int idx = 2; idx > 0; --idx)
    {
        pte_t* pte = &pagetable[ADDR_IDX(virt_addr, idx)];
        if((*pte) & PTE_PERMISSION_V)
        {
            pagetable = (pagetable_t)(PTE_TO_PHY_ADDR(*pte));
        }
        else
        {
            pagetable = (pagetable_t)alloc_single_page();
            memset(pagetable, 0, PAGE_SIZE);
            if(pagetable == NULL)
            {
                printf("pagetable NULL\n\r");
                return NULL;
            }
            (*pte) = PHY_ADDR_TO_PTE(pagetable) | PTE_PERMISSION_V;
        }
    }
    return &pagetable[ADDR_IDX(virt_addr, 0)];
}

void set_vm_pagetable(pagetable_t pagetable)
{
    sfence_vma();
    w_satp((SATP_SV39_MODE << RV64_SATP_MODE_OFFSET) | PHY_TO_SATP((addr_t)pagetable));
    sfence_vma();
}

/****************************************************************************************************************************************************
 * Serial I/O related functions.
****************************************************************************************************************************************************/
void print_char(char c)
{
    uart_print_char(c);
}

void print_str(char *str)
{
    uart_print_str(str);
}

int scan_char()
{
    return uart_scan_char();
}
