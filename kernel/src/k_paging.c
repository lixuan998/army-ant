#include "arch/defs.h"
#include "lib/include/stdio.h"
#include "lib/include/stdlib.h"

SPINLOCK mem_paging_spinlock;

MEM_PAGE *mem_page_list = NULL;

void mem_paging_init()
{
    spinlock_init(&mem_paging_spinlock, "mem_paging_spinlock");
    free_pages((addr_t *)(kernel_end), (addr_t *)(RAM_TOP));    
}

void *alloc_single_page()
{
    if(mem_page_list == NULL)
    {
        panic("in alloc_single_page, no memory page left");
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
    if((addr_t)page % PAGE_SIZE || (addr_t )page < (addr_t )kernel_end || (addr_t)page > RAM_TOP)
    {
        panic("in free_single_page, page out of bound");
    }
    memset(page, UNALLOCTE_PAGE_VALUE, PAGE_SIZE);

    spinlock_lock(&mem_paging_spinlock);
    MEM_PAGE *new_page = (MEM_PAGE *)page;
    new_page->next = mem_page_list;
    mem_page_list = new_page;
    spinlock_unlock(&mem_paging_spinlock);
}

void free_pages(void *p_start, void *p_end)
{
    addr_t pg = (addr_t)ALIGN_FLOOR((addr_t)p_end);
    while(pg - PAGE_SIZE >= (addr_t )p_start)
    {
        free_single_page((void *)pg);
        pg -= PAGE_SIZE;
    }
}
