#pragma once

#include <lib/stdint.h>
#include <lib/stddef.h>
#include <common/lock.h>
#include <common/error.h>


#define BITMAP_MAX_WIDTH  2048

typedef struct _bitmap {
    spinlock_t splock;
    char bits[BITMAP_MAX_WIDTH / 8];
    uint32_t bitmap_width;
} bitmap;

error_t bitmap_init(bitmap *bmp, uint32_t width);
error_t bitmap_set(bitmap *bmp, uint32_t bit_idx);
error_t bitmap_clear(bitmap *bmp, uint32_t bit_idx);
error_t bitmap_read(bitmap *bmp, uint32_t bit_idx,
                    uint32_t *value);