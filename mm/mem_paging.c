#include <mm/mem_paging.h>
#include <common/lock.h>
#include <common/operation.h>
#include <arch/mm/arch_mem_layout.h>
#include <lib/memops.h>

static struct spinlock splock;
static MEM_PAGE *mem_pages = NULL;

void mem_paging_init()
{
    spinlock_init(&splock);
    mem_page_range_free((addr_t)__kernel_end, (addr_t)__top_addr);
}

addr_t mem_page_alloc()
{
    if (mem_pages == NULL) {
        PANIC("in alloc_single_page, no memory page left");
    }
    spinlock_lock(&splock);
    MEM_PAGE *page = mem_pages;
    mem_pages = mem_pages->next;
    spinlock_unlock(&splock);

    memset(page, ALLOCATED_PAGE_VALUE, PAGE_SIZE);
    return (addr_t)page;
}

void mem_page_free(addr_t page)
{
    if (page % PAGE_SIZE || page < (addr_t)__kernel_end || page > (addr_t)__top_addr) {
        PANIC("in free_single_page, page out of bound");
    }
    memset((void *)page, FREE_PAGE_VALUE, PAGE_SIZE);

    spinlock_lock(&splock);
    MEM_PAGE *new_page = (MEM_PAGE *)page;
    new_page->next = mem_pages;
    mem_pages = new_page;
    spinlock_unlock(&splock);
}

void mem_page_range_free(addr_t start, addr_t end)
{
    addr_t pg = (addr_t)ALIGN_FLOOR((addr_t)end, PAGE_SIZE);
    while (pg - PAGE_SIZE >= (addr_t)start) {
        pg -= PAGE_SIZE;
        mem_page_free(pg);
    }
}
