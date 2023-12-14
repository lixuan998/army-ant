#include "kernel/include/k_stdlib.h"

void k_memset(void *ptr, int value, int size)
{
    char *cptr = (char *)ptr;
    for(int i = 0; i < size; ++ i)
    {
        cptr[i] = value;
    }
}