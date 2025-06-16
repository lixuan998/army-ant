#include <mm/static_mem.h>
#include <common/lock.h>
#include <common/log.h>
#include <arch/mm/arch_mem_layout.h>
#include <lib/memops.h>
#include <lib/bitmap.h>

static spinlock_t _sm_splock;
static char static_mem_pool[STATIC_MEM_PAGES][PAGE_SIZE] __attribute__((aligned(PAGE_SIZE)));
static bitmap static_mem_bitmap;

void static_mem_init()
{
    spinlock_init(&_sm_splock);
    error_t ret = bitmap_init(&static_mem_bitmap, STATIC_MEM_PAGES);
    if (ret != AA_ERROR_SUCCESS) {
        PANIC("static memory bitmap init failed");
    }
}
addr_t static_mem_alloc()
{
    spinlock_lock(&_sm_splock);
    addr_t mm_pg_addr = 0;
    uint32_t bitmap_val;
    for (int i = 0; i < STATIC_MEM_PAGES; ++i) {
        error_t err = bitmap_read(&static_mem_bitmap, i,
                                  &bitmap_val);
        if (err != AA_ERROR_SUCCESS) {
            KLOG_ERR("STATIC MEMORY", "bitmap read failed");
            goto err;
        }
        if (bitmap_val == 0) {
            bitmap_set(&static_mem_bitmap, i);
            mm_pg_addr = (addr_t)(static_mem_pool + i);
            break;
        }
    }
err:
    spinlock_unlock(&_sm_splock);
    return mm_pg_addr;
}

void static_mem_free(addr_t mm_pg_addr)
{
    spinlock_lock(&_sm_splock);
    bitmap_clear(&static_mem_bitmap, (mm_pg_addr - (addr_t)static_mem_pool) / PAGE_SIZE);
    memset(static_mem_pool[(mm_pg_addr - (addr_t)static_mem_pool) / PAGE_SIZE], 0, PAGE_SIZE);
    spinlock_unlock(&_sm_splock);
}