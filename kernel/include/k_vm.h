#ifndef __K_VM_H__
#define __K_VM_H__

#include "arch/defs.h"
#include "lib/include/stdio.h"
#include "k_paging.h"

void k_pagetable_init();
void k_vm_enable();

#endif /* __K_VM_H__ */