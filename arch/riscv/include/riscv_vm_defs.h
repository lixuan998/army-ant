#ifndef __RISCV_VM_DEFS_H__
#define __RISCV_VM_DEFS_H__

#include "riscv_type_defs.h"
#include "riscv_mem_defs.h"
#include "riscv_mem_layout.h"

pagetable_t pagetable_create(VM_MAP_INFO map_info[], int num_of_mapping);
void pagetable_destroy(pagetable_t pagetable);
void pagetable_entry_add(pagetable_t pagetable, VM_MAP_INFO map_info[], int num_of_mapping);
int vm_mapping(pagetable_t pagetable, addr_t virt_addr_start, addr_t phys_addr_start, pagesize_t size, isa_reg_t permisson);
pte_t* pte_retrieve(pagetable_t pagetable, addr_t virt_addr);
void set_vm_pagetable(pagetable_t pagetable);

extern HEAP_MEM_BLK *mem_blk_list;

#endif  /* __RISCV_VM_DEFS_H__ */