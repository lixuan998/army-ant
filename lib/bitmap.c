#include <lib/bitmap.h>
#include <lib/memops.h>
#include <lib/printk.h>

error_t bitmap_init(bitmap *bmp, uint32_t width)
{
    spinlock_init(&bmp->splock);
    if(width > BITMAP_MAX_WIDTH)
        return AA_ERROR_BITMAP_OVERFLOW;
    bmp->bitmap_width = width;
    memset(bmp->bits, 0, BITMAP_MAX_WIDTH / 8);
    return AA_ERROR_SUCCESS;
}

error_t bitmap_set(bitmap *bmp, uint32_t bit_idx)
{
    
    if(bit_idx > bmp->bitmap_width)
        return AA_ERROR_BITMAP_OVERFLOW;
    spinlock_lock(&bmp->splock);
    bmp->bits[bit_idx / 8] |= (1 << (bit_idx % 8));
    spinlock_unlock(&bmp->splock);
    return AA_ERROR_SUCCESS;
}

error_t bitmap_clear(bitmap *bmp, uint32_t bit_idx)
{
    if(bit_idx > bmp->bitmap_width)
        return AA_ERROR_BITMAP_OVERFLOW;
    spinlock_lock(&bmp->splock);
    bmp->bits[bit_idx / 8] &= ~(1 << (bit_idx % 8));
    spinlock_unlock(&bmp->splock);
    return AA_ERROR_SUCCESS;
}

error_t bitmap_read(bitmap *bmp, uint32_t bit_idx,
                    uint32_t *value)
{
    if(bit_idx > bmp->bitmap_width)
        return AA_ERROR_BITMAP_OVERFLOW;
    *value = bmp->bits[bit_idx / 8] & (1 << (bit_idx % 8));
    return AA_ERROR_SUCCESS;
}