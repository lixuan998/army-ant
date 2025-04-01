#pragma once

#include <lib/stdint.h>
#include <lib/stddef.h>

void memset(void *ptr, int value, int size);

void memcpy(char *dest, char *src, int size);

int memcmp(void *s1, void *s2, int n);

char *strchr(char *s, int c);