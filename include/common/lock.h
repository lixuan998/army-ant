#pragma once

#include <lib/stdint.h>
#include <lib/stddef.h>
#include <common/error.h>

struct spinlock {
    union {
        struct {
            uint32_t owner;
            uint32_t next;
        };
        uint64_t slock;
    };
};

typedef struct spinlock spinlock_t;

void spinlock_init(spinlock_t *splock);
void spinlock_lock(spinlock_t *splock);
void spinlock_unlock(spinlock_t *splock);
error_t spinlock_try_lock(spinlock_t *splock);