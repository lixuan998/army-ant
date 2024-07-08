#define __SYSCALL_INTERFACE_S__
#include "arch/riscv/include/riscv_syscall_defs.h"

.global syscall_exit
syscall_exit:
    li a7, SYSCALL_EXIT
    ecall
    ret