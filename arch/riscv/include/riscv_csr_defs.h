#ifndef __RISCV_CSR_DEFS_H__
#define __RISCV_CSR_DEFS_H__

#define REGISTER_NUMBERS                      32

#define USER_MODE_CODE                        0
#define SUPERVISOR_MODE_CODE                  1
#define HYPERVISOR_MODE_CODE                  2
#define MACHINE_MODE_CODE                     3

#define MSTATUS_SIE_OFFSET                    1
#define MSTATUS_SIE_MASK                      1UL << MSTATUS_SIE_OFFSET
#define MSTATUS_MIE_OFFSET                    3
#define MSTATUS_MIE_MASK                      1UL << MSTATUS_MIE_OFFSET
#define MSTATUS_SPIE_OFFSET                   5
#define MSTATUS_SPIE_MASK                     1UL << MSTATUS_SPIE_OFFSET
#define MSTATUS_MPIE_OFFSET                   7
#define MSTATUS_MPIE_MASK                     1UL << MSTATUS_MPIE_OFFSET
#define MSTATUS_SPP_OFFSET                    8
#define MSTATUS_SPP_MASK                      1UL << MSTATUS_SPP_OFFSET
#define MSTATUS_MPP_OFFSET                    11UL
#define MSTATUS_MPP_MASK                      3UL << MSTATUS_MPP_OFFSET

#define SSTATUS_SIE_OFFSET                    1
#define SSTATUS_SIE_MASK                      1UL << SSTATUS_SIE_OFFSET
#define SSTATUS_SPIE_OFFSET                   5
#define SSTATUS_SPIE_MASK                     1UL << SSTATUS_SPIE_OFFSET
#define SSTATUS_SPP_OFFSET                    8
#define SSTATUS_SPP_MASK                      (1UL << SSTATUS_SPP_OFFSET)

#endif  /* __RISCV_CSR_DEFS_H__ */