#include "../include/stdlib.h"

MEM_BLK *kernel_mem_blk_head;
MEM_BLK *mem_blk_list = NULL;

SPINLOCK malloc_lock;

void memset(void *ptr, int value, int size)
{
    char *cptr = (char *)ptr;
    for(int i = 0; i < size; ++ i)
    {
        cptr[i] = value;
    }
}

void memcpy(void *dest, void *src, int size)
{
    char *cdest = (char *)dest;
    char *csrc = (char *)src;
    char tmp[size];
    for(int i = 0; i < size; ++ i)
    {
        tmp[i] = csrc[i];
    }
    for(int i = 0; i < size; ++ i)
    {
        cdest[i] = tmp[i];
    }
    return;
}

void blk_init(MEM_BLK *blk)
{
    blk->blk_free_space = PAGE_SIZE - sizeof(MEM_BLK);

    for(int i = 0; i < SQRT_PAGE_SIZE; ++ i)
    {
        memset(blk -> blk_cell[i], 0, SQRT_PAGE_SIZE / bitof(char));
    }
    
    for(int cell_num = 0; cell_num < sizeof(MEM_BLK); ++ cell_num)
    {
        blk_cell_use(blk, cell_num);
    }

    blk->next = NULL;  
    return;
}

void blk_cell_use(MEM_BLK *blk, int cell_num)
{
    int x = cell_num / SQRT_PAGE_SIZE;
    int y = cell_num % SQRT_PAGE_SIZE;
    
    if(x >= SQRT_PAGE_SIZE || y >= SQRT_PAGE_SIZE)
    {
        panic("in blk_cell_use: cell x or cell y overrange.");
    }
    int blk_cell_y_idx = y / bitof(char);

    int blk_cell_y_offset = y % bitof(char);

    blk -> blk_cell[x][blk_cell_y_idx] |= (1 << blk_cell_y_offset);
}

int is_blk_cell_used(MEM_BLK *blk, int cell_num)
{
    // printf("cell num: %d\n\r", cell_num);
    int x = cell_num / SQRT_PAGE_SIZE;
    int y = cell_num % SQRT_PAGE_SIZE;
    
    if(x >= SQRT_PAGE_SIZE || y >= SQRT_PAGE_SIZE)
    {
        printf("x: %d, y: %d\n\r", x, y);
        panic("in is_blk_cell_used: cell x or cell y overrange.");
    }
    int blk_cell_y_idx = y / bitof(char);

    int blk_cell_y_offset = y % bitof(char);

    int cell_used = ((blk -> blk_cell[x][blk_cell_y_idx]) >> blk_cell_y_offset) & 1;

    return cell_used;
}


void blk_cell_free(MEM_BLK *blk, int cell_num)
{
    int x = cell_num / SQRT_PAGE_SIZE;
    int y = cell_num % SQRT_PAGE_SIZE;
    
    if(x >= SQRT_PAGE_SIZE || y >= SQRT_PAGE_SIZE)
    {
        panic("in blk_cell_free: cell x or cell y overrange.");
    }
    int blk_cell_y_idx = y / bitof(char);

    int blk_cell_y_offset = y % bitof(char);

    blk -> blk_cell[x][blk_cell_y_idx] &= (~(1 << blk_cell_y_offset));
}

void malloc_init()
{
    spinlock_init(&malloc_lock, "malloc_spinlock");
    kernel_mem_blk_head = (MEM_BLK *)alloc_single_page();
    blk_init(kernel_mem_blk_head);
    mem_blk_list = kernel_mem_blk_head;
}

void *malloc(int size)
{
    printf("malloc called, size: %d\n\r", size);
    if(mem_blk_list == NULL)
    {
        malloc_init();
    }

    addr_t ret_addr = NULL;
    MEM_BLK *cur_blk = mem_blk_list;
    
    while(ret_addr == NULL)
    {
        spinlock_lock(&malloc_lock);
        
        if(cur_blk -> blk_free_space >= size + META_DATA_SIZE)
        {
            int continue_cells;
            int cell_num = 0;
            while(is_blk_cell_used(cur_blk, cell_num))
            {
                cell_num ++;
            }
            for(; cell_num < PAGE_SIZE - (size + META_DATA_SIZE); ++ cell_num)
            {
                continue_cells = 0;
                for(int i = 0; i < size + META_DATA_SIZE; ++ i)
                {
                    if(!is_blk_cell_used(cur_blk, cell_num + i))
                    {
                        continue_cells ++;
                    }
                    else
                    {
                        continue_cells = 0;
                        break;
                    }
                }
                if(continue_cells == size + META_DATA_SIZE)
                {
                    memcpy((void *)((addr_t)cur_blk + (addr_t)cell_num), &size, META_DATA_SIZE);
                    ret_addr = (addr_t)cur_blk + (addr_t)cell_num + (addr_t)META_DATA_SIZE;
                    cur_blk -> blk_free_space -= (size + META_DATA_SIZE);
                    for(int i = 0; i < size + META_DATA_SIZE; ++ i)
                    {
                        blk_cell_use(cur_blk, cell_num + i);
                    }
                    break;
                }
            }
        }
        else
        {
            if(cur_blk -> next == NULL)
            {
                cur_blk -> next = alloc_single_page();
                blk_init(cur_blk -> next);
            }
            // printf("cur next: %x mem list next: %x\n\r", cur_blk -> next, mem_blk_list -> next);
            cur_blk = cur_blk -> next;
        }
        spinlock_unlock(&malloc_lock);
    }
    
    debug_malloc(cur_blk);
    return (void *)ret_addr;
}

void free(void *ptr)
{
    MEM_BLK *cur_blk = mem_blk_list;
    while(cur_blk != NULL)
    {
        if((addr_t)cur_blk == (addr_t)ALIGN_FLOOR((addr_t)ptr)) break;
        cur_blk = cur_blk -> next;
    }
    int ptr_size = *((int *)((addr_t)ptr - META_DATA_SIZE));
    int cell_num = (addr_t)ptr - (addr_t)cur_blk - META_DATA_SIZE;
    for(int i = 0; i < ptr_size + META_DATA_SIZE; ++ i)
    {
        blk_cell_free(cur_blk, cell_num + i);
    }
    cur_blk -> blk_free_space += (ptr_size + META_DATA_SIZE);
    debug_malloc(cur_blk);
}

void debug_malloc(MEM_BLK *blk)
{
    int y_bit;
    printf("remain space: %d\n\r", blk -> blk_free_space);
    for(int i = 0; i < SQRT_PAGE_SIZE; ++ i)
    {
        for(int j = 0; j < SQRT_PAGE_SIZE; ++ j)
        {
            y_bit = ((blk -> blk_cell[i][j / bitof(char)]) >> (j % bitof(char)) & 1);
            printf("%d ", y_bit);
        }
        printf("\n\r");
    }
}
