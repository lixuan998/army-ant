#ifndef __RISCV_TYPE_DEFS_H__
#define __RISCV_TYPE_DEFS_H__

typedef unsigned int                          uint;
typedef unsigned short                        ushort;
typedef unsigned char                         uchar;

typedef unsigned char                         uint8;
typedef unsigned short                        uint16;
typedef unsigned int                          uint32;
typedef unsigned long                         uint64;

typedef uint64                                isa_reg_t;
typedef uint64                                ptr_t;

typedef uint64*                               pagetable_t;
typedef uint64                                pte_t;
typedef uint64                                addr_t;
typedef uint64                                pagesize_t;

#define NULL                                  0

#endif /* __RISCV_TYPE_DEFS_H__ */