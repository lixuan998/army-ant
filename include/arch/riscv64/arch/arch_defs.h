#pragma once

#include <lib/stdint.h>

/**
 * @brief CSR Related Defines.
 * 
 */
#define REGISTER_NUMBERS                      32

#define USER_MODE_CODE                        0
#define SUPERVISOR_MODE_CODE                  1
#define HYPERVISOR_MODE_CODE                  2
#define MACHINE_MODE_CODE                     3

#define MSTATUS_SIE_OFFSET                    1
#define MSTATUS_SIE_MSK                       1UL << MSTATUS_SIE_OFFSET
#define MSTATUS_MIE_OFFSET                    3
#define MSTATUS_MIE_MSK                       1UL << MSTATUS_MIE_OFFSET
#define MSTATUS_SPIE_OFFSET                   5
#define MSTATUS_SPIE_MSK                      1UL << MSTATUS_SPIE_OFFSET
#define MSTATUS_MPIE_OFFSET                   7
#define MSTATUS_MPIE_MSK                      1UL << MSTATUS_MPIE_OFFSET
#define MSTATUS_SPP_OFFSET                    8
#define MSTATUS_SPP_MSK                       1UL << MSTATUS_SPP_OFFSET
#define MSTATUS_MPP_OFFSET                    11
#define MSTATUS_MPP_MSK                       (3UL << MSTATUS_MPP_OFFSET)

#define SSTATUS_SIE_OFFSET                    1
#define SSTATUS_SIE_MSK                       (1UL << SSTATUS_SIE_OFFSET)
#define SSTATUS_SPIE_OFFSET                   5
#define SSTATUS_SPIE_MSK                      (1UL << SSTATUS_SPIE_OFFSET)
#define SSTATUS_SPP_OFFSET                    8
#define SSTATUS_SPP_MSK                       (1UL << SSTATUS_SPP_OFFSET)

/**
 * @brief Interrupt Related Defines.
 * 
 */
#define xIE_MSIE_OFFSET                       3
#define xIE_MSIE_MSK                          (1UL << xIE_MSIE_OFFSET)
#define xIE_MTIE_OFFSET                       7
#define xIE_MTIE_MSK                          (1UL << xIE_MTIE_OFFSET)
#define xIE_MEIE_OFFSET                       11
#define xIE_MEIE_MSK                          (1UL << xIE_MEIE_OFFSET)

#define xIE_SSIE_OFFSET                       1
#define xIE_SSIE_MSK                          (1UL << xIE_SSIE_OFFSET)
#define xIE_STIE_OFFSET                       5
#define xIE_STIE_MSK                          (1UL << xIE_STIE_OFFSET)
#define xIE_SEIE_OFFSET                       9
#define xIE_SEIE_MSK                          (1UL << xIE_SEIE_OFFSET)

#define SCAUSE_INTERRUPT_OFFSET               63
#define SCAUSE_INTERRUPT_MSK                  (1UL << 63)
#define SCAUSE_EXCEPTION_CODE_MSK             0x7FFFFFFFFFFFFFFFUL

typedef uintptr_t addr_t;