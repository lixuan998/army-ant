#include "../include/stdlib.h"

HEAP_MEM_BLK *kernel_mem_blk_head;
HEAP_MEM_BLK *mem_blk_list = NULL;

SPINLOCK malloc_lock;

void memset(void *ptr, int value, int size)
{
    char *cptr = (char *)ptr;
    for(int i = 0; i < size; ++ i)
    {
        cptr[i] = value;
    }
}

void memcpy(char *dest, char *src, int size)
{
    if(size == 14) printf("Equal\n\r");
    for(int i = 0; i < size; ++ i)
    {
        dest[i] = src[i];
    }
}

// void *malloc(int size)
// {
//     addr_t ret_addr = NULL;
//     HEAP_MEM_BLK *cur_heap_mem_blk = current_cpu_proc() -> trapframe -> heap_mem_blk;
//     spinlock_init(&malloc_lock, "malloc_spinlock");
    
//     while(ret_addr == NULL)
//     {
//         spinlock_lock(&malloc_lock);
        
//         if(cur_heap_mem_blk -> blk_free_space >= size + META_DATA_SIZE)
//         {
//             int continue_cells;
//             int cell_num = 0;
//             while(is_heap_mem_blk_cell_used(cur_heap_mem_blk, cell_num))
//             {
//                 cell_num ++;
//             }
//             for(; cell_num < PAGE_SIZE - (size + META_DATA_SIZE); ++ cell_num)
//             {
//                 continue_cells = 0;
//                 for(int i = 0; i < size + META_DATA_SIZE; ++ i)
//                 {
//                     if(!is_heap_mem_blk_cell_used(cur_heap_mem_blk, cell_num + i))
//                     {
//                         continue_cells ++;
//                     }
//                     else
//                     {
//                         continue_cells = 0;
//                         break;
//                     }
//                 }
//                 if(continue_cells == size + META_DATA_SIZE)
//                 {
//                     memcpy((void *)((addr_t)cur_heap_mem_blk + (addr_t)cell_num), &size, META_DATA_SIZE);
//                     ret_addr = (addr_t)cur_heap_mem_blk + (addr_t)cell_num + (addr_t)META_DATA_SIZE;
//                     cur_heap_mem_blk -> blk_free_space -= (size + META_DATA_SIZE);
//                     for(int i = 0; i < size + META_DATA_SIZE; ++ i)
//                     {
//                         heap_mem_blk_cell_use(cur_heap_mem_blk, cell_num + i);
//                     }
//                     break;
//                 }
//             }
//         }
//         else
//         {
//             if(cur_heap_mem_blk -> next == NULL)
//             {
//                 cur_heap_mem_blk -> next = alloc_single_page();
//                 heap_mem_blk_init(cur_heap_mem_blk -> next);
//             }
//             // printf("cur next: %x mem list next: %x\n\r", cur_heap_mem_blk -> next, mem_blk_list -> next);
//             cur_heap_mem_blk = cur_heap_mem_blk -> next;
//         }
//         spinlock_unlock(&malloc_lock);
//     }
    
//     // debug_malloc(cur_heap_mem_blk);
//     return (void *)ret_addr;
// }

// void free(void *ptr)
// {
//     HEAP_MEM_BLK *cur_heap_mem_blk = current_cpu_proc() -> trapframe -> heap_mem_blk;
//     while(cur_heap_mem_blk != NULL)
//     {
//         if((addr_t)cur_heap_mem_blk == (addr_t)ALIGN_FLOOR((addr_t)ptr)) break;
//         cur_heap_mem_blk = cur_heap_mem_blk -> next;
//     }
//     int ptr_size = *((int *)((addr_t)ptr - META_DATA_SIZE));
//     int cell_num = (addr_t)ptr - (addr_t)cur_heap_mem_blk - META_DATA_SIZE;
//     for(int i = 0; i < ptr_size + META_DATA_SIZE; ++ i)
//     {
//         heap_mem_blk_cell_free(cur_heap_mem_blk, cell_num + i);
//     }
//     cur_heap_mem_blk -> blk_free_space += (ptr_size + META_DATA_SIZE);
//     // debug_malloc(cur_heap_mem_blk);
// }

void debug_malloc(HEAP_MEM_BLK *blk)
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
