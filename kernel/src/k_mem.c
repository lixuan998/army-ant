#include "arch/riscv/include/riscv_mem_defs.h"
#include "lib/include/stdio.h"
#include "lib/include/stdlib.h"

SPINLOCK mem_paging_spinlock;

MEM_PAGE *mem_page_list = NULL;

HEAP_MEM_BLK *k_heap_mem_blk = NULL;
SPINLOCK k_malloc_spinlock;

void mem_paging_init()
{
    spinlock_init(&mem_paging_spinlock, "mem_paging_spinlock");
    free_pages((addr_t)(kernel_end_addr), (addr_t)(MEM_TOP_ADDR));
}

addr_t allocate_shared_memory(PROC *procs[], int proc_num)
{
    addr_t shared_memory_page_addr = (addr_t)alloc_single_page();
    int *reference_cnt = (int *)(shared_memory_page_addr + PAGE_SIZE - sizeof(int));
    *reference_cnt = 0;
    VM_MAP_INFO shared_memory_map_info[] = {
        {.virt_addr_start = (addr_t)(VM_PROC_SHARED_MEM_ADDR),
         .phys_addr_start = shared_memory_page_addr,
         .size = PAGE_SIZE,
         .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_X | PTE_PERMISSION_D | PTE_PERMISSION_A | PTE_PERMISSION_U}};

    for (int i = 0; i < proc_num; ++i)
    {
        pagetable_entry_add(procs[i]->pagetable, shared_memory_map_info, 1);
        *reference_cnt += 1;
        procs[i]->shared_mem_addr = shared_memory_page_addr;
    }
    printf("in allocate_shared_memory, reference_cnt: %d\n", *reference_cnt);
    return shared_memory_page_addr;
}

void *alloc_single_page()
{
    if (mem_page_list == NULL)
    {
        panic("in alloc_single_page, no memory page left");
    }
    spinlock_lock(&mem_paging_spinlock);
    MEM_PAGE *page = mem_page_list;
    mem_page_list = mem_page_list->next;
    spinlock_unlock(&mem_paging_spinlock);

    memset(page, ALLOCATED_PAGE_VALUE, PAGE_SIZE);
    return (void *)page;
}

void free_single_page(addr_t page)
{
    if ((addr_t)page % PAGE_SIZE || (addr_t)page < (addr_t)kernel_end_addr || (addr_t)page > MEM_TOP_ADDR)
    {
        panic("in free_single_page, page out of bound");
    }
    memset((void *)page, UNALLOCTE_PAGE_VALUE, PAGE_SIZE);

    spinlock_lock(&mem_paging_spinlock);
    MEM_PAGE *new_page = (MEM_PAGE *)page;
    new_page->next = mem_page_list;
    mem_page_list = new_page;
    spinlock_unlock(&mem_paging_spinlock);
}

void free_pages(addr_t p_start, addr_t p_end)
{
    addr_t pg = (addr_t)ALIGN_FLOOR((addr_t)p_end);
    while (pg - PAGE_SIZE >= (addr_t)p_start)
    {
        free_single_page(pg);
        pg -= PAGE_SIZE;
    }
}

void heap_mem_blk_init(HEAP_MEM_BLK *blk)
{
    blk->blk_free_space = PAGE_SIZE - sizeof(HEAP_MEM_BLK);

    for (int i = 0; i < SQRT_PAGE_SIZE; ++i)
    {
        memset(blk->blk_cell[i], 0, SQRT_PAGE_SIZE / bitof(char));
    }

    for (int cell_num = 0; cell_num < sizeof(HEAP_MEM_BLK); ++cell_num)
    {
        heap_mem_blk_cell_use(blk, cell_num);
    }

    blk->next = NULL;
    return;
}

void heap_mem_blk_cell_use(HEAP_MEM_BLK *blk, int cell_num)
{
    int x = cell_num / SQRT_PAGE_SIZE;
    int y = cell_num % SQRT_PAGE_SIZE;

    if (x >= SQRT_PAGE_SIZE || y >= SQRT_PAGE_SIZE)
    {
        panic("in blk_cell_use: cell x or cell y overrange.");
    }
    int blk_cell_y_idx = y / bitof(char);

    int blk_cell_y_offset = y % bitof(char);

    blk->blk_cell[x][blk_cell_y_idx] |= (1 << blk_cell_y_offset);
}

int is_heap_mem_blk_cell_used(HEAP_MEM_BLK *blk, int cell_num)
{
    int x = cell_num / SQRT_PAGE_SIZE;
    int y = cell_num % SQRT_PAGE_SIZE;

    if (x >= SQRT_PAGE_SIZE || y >= SQRT_PAGE_SIZE)
    {
        printf("x: %d, y: %d\n\r", x, y);
        panic("in is_blk_cell_used: cell x or cell y overrange.");
    }
    int blk_cell_y_idx = y / bitof(char);

    int blk_cell_y_offset = y % bitof(char);

    int cell_used = ((blk->blk_cell[x][blk_cell_y_idx]) >> blk_cell_y_offset) & 1;

    return cell_used;
}

void heap_mem_blk_cell_free(HEAP_MEM_BLK *blk, int cell_num)
{
    int x = cell_num / SQRT_PAGE_SIZE;
    int y = cell_num % SQRT_PAGE_SIZE;

    if (x >= SQRT_PAGE_SIZE || y >= SQRT_PAGE_SIZE)
    {
        panic("in blk_cell_free: cell x or cell y overrange.");
    }
    int blk_cell_y_idx = y / bitof(char);

    int blk_cell_y_offset = y % bitof(char);

    blk->blk_cell[x][blk_cell_y_idx] &= (~(1 << blk_cell_y_offset));
}

void *k_malloc(int size)
{
    addr_t ret_addr = NULL;
    if (k_heap_mem_blk == NULL)
    {
        k_heap_mem_blk = (HEAP_MEM_BLK *)alloc_single_page();
        heap_mem_blk_init(k_heap_mem_blk);
    }
    HEAP_MEM_BLK *cur_heap_mem_blk = k_heap_mem_blk;
    spinlock_init(&k_malloc_spinlock, "k_malloc_lock");

    while (ret_addr == NULL)
    {
        spinlock_lock(&k_malloc_spinlock);

        if (cur_heap_mem_blk->blk_free_space >= size + META_DATA_SIZE)
        {
            int continue_cells;
            int cell_num = 0;
            while (is_heap_mem_blk_cell_used(cur_heap_mem_blk, cell_num))
            {
                cell_num++;
            }
            for (; cell_num < PAGE_SIZE - (size + META_DATA_SIZE); ++cell_num)
            {
                continue_cells = 0;
                for (int i = 0; i < size + META_DATA_SIZE; ++i)
                {
                    if (!is_heap_mem_blk_cell_used(cur_heap_mem_blk, cell_num + i))
                    {
                        continue_cells++;
                    }
                    else
                    {
                        continue_cells = 0;
                        break;
                    }
                }
                if (continue_cells == size + META_DATA_SIZE)
                {
                    memcpy((char *)((addr_t)cur_heap_mem_blk + (addr_t)cell_num), (char *)&size, META_DATA_SIZE);
                    ret_addr = (addr_t)cur_heap_mem_blk + (addr_t)cell_num + (addr_t)META_DATA_SIZE;
                    cur_heap_mem_blk->blk_free_space -= (size + META_DATA_SIZE);
                    for (int i = 0; i < size + META_DATA_SIZE; ++i)
                    {
                        heap_mem_blk_cell_use(cur_heap_mem_blk, cell_num + i);
                    }
                    break;
                }
            }
        }
        else
        {
            if (cur_heap_mem_blk->next == NULL)
            {
                cur_heap_mem_blk->next = alloc_single_page();
                heap_mem_blk_init(cur_heap_mem_blk->next);
            }
            // printf("cur next: %x mem list next: %x\n\r", cur_heap_mem_blk -> next, mem_blk_list -> next);
            cur_heap_mem_blk = cur_heap_mem_blk->next;
        }
        spinlock_unlock(&k_malloc_spinlock);
    }

    // printf("K_MALLOC\n\r");
    // debug_malloc(cur_heap_mem_blk);
    return (void *)ret_addr;
}

void k_free(void *ptr)
{
    if (k_heap_mem_blk == NULL)
    {
        panic("k_free: heap mem blk is null");
    }
    HEAP_MEM_BLK *cur_heap_mem_blk = k_heap_mem_blk;

    while (cur_heap_mem_blk != NULL)
    {
        if ((addr_t)cur_heap_mem_blk == (addr_t)ALIGN_FLOOR((addr_t)ptr))
            break;
        cur_heap_mem_blk = cur_heap_mem_blk->next;
    }
    int ptr_size = *((int *)((addr_t)ptr - META_DATA_SIZE));
    memset(ptr - META_DATA_SIZE, 0, ptr_size + META_DATA_SIZE);
    int cell_num = (addr_t)ptr - (addr_t)cur_heap_mem_blk - META_DATA_SIZE;
    for (int i = 0; i < ptr_size + META_DATA_SIZE; ++i)
    {
        heap_mem_blk_cell_free(cur_heap_mem_blk, cell_num + i);
    }
    cur_heap_mem_blk->blk_free_space += (ptr_size + META_DATA_SIZE);

    // printf("K_FREE\n\r");
    // debug_malloc(cur_heap_mem_blk);
}