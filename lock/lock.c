#include <common/lock.h>
#include <common/operation.h>
#include <arch/atomic.h>

void spinlock_init(spinlock *splock)
{
    if(!splock)
        panic("splock is NULL");
    splock->owner = 0;
    splock->next = 0;
}

void spinlock_lock(spinlock *splock)
{
    uint32_t cur_val = 0;
    if(!splock)
        panic("splock is NULL");
    cur_val = atomic_fetch_add_32(&splock->next, 1);
    while(cur_val != splock->owner);
    __COMPILER_BARRIER();
}

void spinlock_unlock(spinlock *splock)
{
    if(!splock){
        panic("splock is NULL");
	}
    __COMPILER_BARRIER();
    splock->owner += 1;
}

error_t spinlock_try_lock(spinlock *splock)
{
    volatile uint32_t new_val = splock->next;
	error_t ret = AA_ERROR_SUCCESS;

    if(!splock) {
        panic("splock is NULL");
    }
	if(splock->owner != new_val
	    || atomic_cmpxchg_32((int *) &splock->next, new_val,
					         new_val + 1) != new_val) {
        ret = AA_ERROR_SPINLOCK_TRY_LOCK;
    }
    
	__COMPILER_BARRIER();
	return ret;
}
