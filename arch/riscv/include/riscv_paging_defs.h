#ifndef __RISCV_PAGING_DEFS_H__
#define __RISCV_PAGING_DEFS_H__

#include "riscv_type_defs.h"
#include "riscv_mem_layout.h"

#define ALIGN_FLOOR(addr)                     (((addr)) & ~(PAGE_SIZE - 1))
#define ALIGN_CEIL(addr)                      (((addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

#define RV32_SATP_MODE_OFFSET                 30
#define RV64_SATP_MODE_OFFSET                 60
#define SATP_BARE_MODE                        0UL
#define SATP_SV32_MODE                        1UL
#define SATP_SV39_MODE                        8UL
#define SATP_SV48_MODE                        9UL

// Provided in linkld.ld
extern addr_t                                 kernel_start_addr[];
extern addr_t                                 kernel_end_addr[];
extern addr_t                                 kernel_text_start_addr[];
extern addr_t                                 kernel_text_end_addr[];
extern addr_t                                 rodata_end_addr[];
extern addr_t                                 data_end_addr[];
extern addr_t                                 bss_start_addr[];
extern addr_t                                 bss_end_addr[];
extern addr_t                                 trampoline_start_addr[];
extern addr_t                                 trampoline_end_addr[];
extern addr_t                                 trampoline[];

#define UNALLOCTE_PAGE_VALUE                  0xFF
#define ALLOCATED_PAGE_VALUE                  0x00

typedef struct _MEM_PAGE{
    struct _MEM_PAGE *next;
} MEM_PAGE;

typedef struct _HEAP_MEM_BLK{
    int blk_free_space;
    char blk_cell[SQRT_PAGE_SIZE][SQRT_PAGE_SIZE / bitof(char)];
    struct _HEAP_MEM_BLK *next;
} HEAP_MEM_BLK;

void *alloc_single_page();
void free_single_page(void *page);
void free_pages(void *p_start, void *p_end);
void mem_paging_init();

void heap_mem_blk_init(HEAP_MEM_BLK *blk);
void heap_mem_blk_cell_use(HEAP_MEM_BLK *blk, int cell_num);
int is_heap_mem_blk_cell_used(HEAP_MEM_BLK *blk, int cell_num);
void heap_mem_blk_cell_free(HEAP_MEM_BLK *blk, int cell_num);

void *k_malloc(int size);
void k_free(void *ptr);

#endif  /* __RISCV_PAGING_DEFS_H__ */