#include <mm/mem_paging.h>
#include <common/lock.h>
#include <common/log.h>
#include <common/operation.h>
#include <arch/mm/arch_mem_layout.h>
#include <lib/memops.h>

static spinlock_t splock;
static struct dl_list mem_page_list;
static addr_t mem_page_meta_base = (addr_t)__kernel_end;

static addr_t reserve_for_meta(void);

const char mm_tag[] = "Memory Page";

void mem_paging_init()
{
    spinlock_init(&splock);
    DL_LIST_INIT(&mem_page_list);
    addr_t mem_page_base = reserve_for_meta();
    mem_page_range_free(mem_page_base, (addr_t)__top_addr);
}

addr_t mem_page_alloc()
{
    spinlock_lock(&splock);
    KLOG_DEBUG(mm_tag, "Allocating page...");
    if (mem_page_list.next == NULL) {
        PANIC("Memory Page Used Up.");
    }
    struct mem_page *_mem_page = DL_LIST_CONTAINER(mem_page_list.next, struct mem_page, mem_page_node);
    DL_LIST_DELETE(&(_mem_page->mem_page_node));

    memset(_mem_page, ALLOCATED_PAGE_VALUE, PAGE_SIZE);
    KLOG_DEBUG(mm_tag, "Alloc page %p", (addr_t)_mem_page);
    spinlock_unlock(&splock);
    return (addr_t)_mem_page;
}

void mem_page_free(addr_t page)
{
    if (page % PAGE_SIZE || page < (addr_t)__kernel_end || page > (addr_t)__top_addr) {
        PANIC("in free_single_page, page out of bound");
    }
    memset((void *)page, FREE_PAGE_VALUE, PAGE_SIZE);

    spinlock_lock(&splock);
    struct mem_page *_mem_page = (struct mem_page *)page;
    DL_LIST_ADD(&mem_page_list, &_mem_page->mem_page_node);
    struct mem_page_meta *_meta = GET_MEM_PAGE_META(_mem_page);
    _meta->size = PAGE_SIZE;
    _meta->used = 0;
    // KLOG_DEBUG(tag, "Free page %p, size: %u, used: %u, meta addr: %p", (addr_t)_mem_page, _meta->size, _meta->used, _meta);
    spinlock_unlock(&splock);
}

void mem_page_range_free(addr_t start, addr_t end)
{
    addr_t pg = end;
    uint32_t _cnt = 0;
    while (pg - PAGE_SIZE >= (addr_t)start) {
        pg -= PAGE_SIZE;
        mem_page_free(pg);
        _cnt ++;
    }
    KLOG_DEBUG(mm_tag, "Free %d pages from address %p to address %p", _cnt, start, end);
}

static addr_t reserve_for_meta(void)
{
    uint32_t cnt = (uint32_t)((addr_t)__top_addr - mem_page_meta_base) / (PAGE_SIZE + (addr_t)sizeof(struct mem_page_meta));
    addr_t mem_page_base = mem_page_meta_base + (addr_t)sizeof(struct mem_page_meta) * cnt;
    if (((addr_t)__top_addr - mem_page_base) / PAGE_SIZE > cnt) {
        PANIC("Memory Page Count Bigger Than Meta Count.");
    }
    return mem_page_base;
}
