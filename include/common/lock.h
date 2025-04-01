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

void spinlock_init(struct spinlock *splock);
void spinlock_lock(struct spinlock *splock);
void spinlock_unlock(struct spinlock *splock);
error_t spinlock_try_lock(struct spinlock *splock);