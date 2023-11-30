#include "../kernel/include/k_logo.h"
#include "../kernel/include/k_stdio.h"
#include "../arch/riscv/include/asm_operation.h"

int main()
{
    uint64 *tp_arr = (uint64 *)r_tp();
    uint64 hartid = tp_arr[0];
    int a = 0;
    char c = 'A';
    k_printf("hardid is : %d test val: %d hex: %x%s %c\n\r", hartid, a, a, "test s", c);
    k_printf(ARMY_ANT_LOGO);
    while(1);
    return 0;
}

