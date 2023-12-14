#include "kernel/include/k_paging.h"

K_SPINLOCK mem_paging_spinlock;
MEM_PAGE *mem_page_list = NULL;

void k_mem_paging_init()
{
    k_spinlock_init(&mem_paging_spinlock, "mem_paging_lock");
    k_free_pages((addr_t *)(kernel_end), (addr_t *)(RAM_TOP));    
}

void k_free_pages(void *p_start, void *p_end)
{
    addr_t *pg = (addr_t*)ALIGN_CEIL((addr_t)p_start);
    for(; pg + PAGE_SIZE <= (addr_t *)p_end; pg += PAGE_SIZE)
    {
        k_free_single_page(pg);
    }
}

void k_free_single_page(void *page)
{
    if((addr_t)page % PAGE_SIZE || (addr_t *)page < kernel_end || (addr_t)page > RAM_TOP)
    {
        k_panic(__FILE__, __LINE__, "in k_free_single_page, page out of bound");
    }
    k_memset(page, UNALLOCTE_PAGE_VALUE, PAGE_SIZE);

    k_spinlock_lock(&mem_paging_spinlock);
    MEM_PAGE *new_page = (MEM_PAGE *)page;
    new_page->next = mem_page_list;
    mem_page_list = new_page;
    k_spinlock_unlock(&mem_paging_spinlock);
}

void *k_alloc_single_page()
{
    if(mem_page_list == NULL)
    {
        k_panic(__FILE__, __LINE__, "in k_alloc_single_page, no memory page left");
    }
    k_spinlock_lock(&mem_paging_spinlock);
    MEM_PAGE *page = mem_page_list;
    mem_page_list = mem_page_list -> next;
    k_spinlock_unlock(&mem_paging_spinlock);

    k_memset(page, ALLOCATED_PAGE_VALUE, PAGE_SIZE);
    return (void *)page;
}
