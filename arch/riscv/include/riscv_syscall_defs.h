#ifndef __RISCV_SYSCALL_DEFS_H__
#define __RISCV_SYSCALL_DEFS_H__

#define SYSCALL_EXIT 0

#ifndef __SYSCALL_INTERFACE_S__

#include "riscv_proc_defs.h"
void syscall();

#endif /* __SYSCALL_INTERFACE_S__ */
#endif /* __RISCV_SYSCALL_DEFS_H__ */