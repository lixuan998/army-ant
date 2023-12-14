#ifndef __K_VM_H__
#define __K_VM_H__

#include "kernel/include/k_defs.h"
#include "kernel/include/k_stdio.h"
#ifdef RV64
#include "arch/riscv/include/riscv_vm.h"
#endif /* RV64 */

int k_vm_map(pagetable_t pagetable, addr_t virt_addr_start, addr_t phys_addr_start, pagesize_t size, uint64 permisson);
void k_pagetable_init();
void k_vm_enable();

#endif /* __K_VM_H__ */