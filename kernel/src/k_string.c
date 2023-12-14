#include "../include/k_string.h"

void k_strcpy(char *dst, const char *src)
{
    while((*src) != '\0')
    {
        *(dst ++) = *(src ++);
    }
    *dst = '\0';
}