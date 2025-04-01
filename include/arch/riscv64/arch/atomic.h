#pragma once

#include <lib/stdint.h>

#define __FENCE(p, s) asm volatile ("fence " #p "," #s : : : "memory")

#define mb()            __FENCE(iorw, iorw)
#define rmb             __FENCE(ir, ir)
#define wmb()           __FENCE(ow, ow)

#define smp_mb()        __FENCE(rw, rw)
#define smp_rmb()       __FENCE(r, r)
#define smp_wmb()       __FENCE(w, w)

#define flush_tlb()     asm volatile("sfence.vma zero, zero");

#define __cmpxchg(ptr, old, new, size)                                    \
	({                                                                \
		__typeof__(ptr) __ptr	 = (ptr);                         \
		__typeof__(*(ptr)) __old = (old);                         \
		__typeof__(*(ptr)) __new = (new);                         \
		__typeof__(*(ptr)) __ret;                                 \
		register unsigned int __rc;                               \
		switch (size) {                                           \
		case 4:                                                   \
			__asm__ __volatile__("0:	lr.w %0, %2\n"    \
					     "	bne  %0, %z3, 1f\n"       \
					     "	sc.w.rl %1, %z4, %2\n"    \
					     "	bnez %1, 0b\n"            \
					     "	fence rw, rw\n"           \
					     "1:\n"                       \
					     : "=&r"(__ret), "=&r"(__rc), \
					       "+A"(*__ptr)               \
					     : "rJ"(__old), "rJ"(__new)   \
					     : "memory");                 \
			break;                                            \
		case 8:                                                   \
			__asm__ __volatile__("0:	lr.d %0, %2\n"    \
					     "	bne %0, %z3, 1f\n"        \
					     "	sc.d.rl %1, %z4, %2\n"    \
					     "	bnez %1, 0b\n"            \
					     "	fence rw, rw\n"           \
					     "1:\n"                       \
					     : "=&r"(__ret), "=&r"(__rc), \
					       "+A"(*__ptr)               \
					     : "rJ"(__old), "rJ"(__new)   \
					     : "memory");                 \
			break;                                            \
		default:                                                  \
			break;                                            \
		}                                                         \
		__ret;                                                    \
	})
// clang-format on

#define cmpxchg(ptr, o, n)                                          \
	({                                                          \
		__typeof__(*(ptr)) _o_ = (o);                       \
		__typeof__(*(ptr)) _n_ = (n);                       \
		(__typeof__(*(ptr)))                                \
			__cmpxchg((ptr), _o_, _n_, sizeof(*(ptr))); \
	})

static inline int atomic_cmpxchg_32(int *ptr, int oldval, int newval)
{
#ifdef __riscv_atomic
	return __sync_val_compare_and_swap(ptr, oldval, newval);
#else
	return cmpxchg(ptr, oldval, newval);
#endif
}

static inline int64_t atomic_cmpxchg_64(int64_t *ptr, int64_t oldval, int64_t newval)
{
#ifdef __riscv_atomic
	return __sync_val_compare_and_swap(ptr, oldval, newval);
#else
	return cmpxchg(ptr, oldval, newval);
#endif
}

#define __COMPILER_BARRIER() asm volatile("":::"memory")

#ifdef __riscv_atomic
#define atomic_fetch_add_32(ptr, val) __sync_fetch_and_add(ptr, val)
#define atomic_fetch_add_64(ptr, val) __sync_fetch_and_add(ptr, val)
#define atomic_fetch_sub_32(ptr, val) __sync_fetch_and_sub(ptr, val)
#define atomic_fetch_sub_64(ptr, val) __sync_fetch_and_sub(ptr, val)
#else
#define atomic_fetch_add_32(ptr, val) BUG("atomic_fetch_and_add not supported.")
#define atomic_fetch_add_64(ptr, val) BUG("atomic_fetch_and_add not supported.")
#define atomic_fetch_sub_32(ptr, val) BUG("atomic_fetch_and_sub not supported.")
#define atomic_fetch_sub_64(ptr, val) BUG("atomic_fetch_and_sub not supported.")
#endif