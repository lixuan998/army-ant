#ifndef __DEFS_H_
#define __DEFS_H_

#define USER_MODE_CODE            0
#define SUPERVISOR_MODE_CODE      1
#define HYPERVISOR_MODE_CODE      2
#define MACHINE_MODE_CODE         3

#define MSTATUS_SIE_OFFSET        1
#define MSTATUS_SIE_MASK          1L << MSTATUS_SIE_OFFSET
#define MSTATUS_MIE_OFFSET        3
#define MSTATUS_MIE_MASK          1L << MSTATUS_MIE_OFFSET
#define MSTATUS_SPIE_OFFSET       5
#define MSTATUS_SPIE_MASK         1L << MSTATUS_SPIE_OFFSET
#define MSTATUS_MPIE_OFFSET       7
#define MSTATUS_MPIE_MASK         1L << MSTATUS_MPIE_OFFSET
#define MSTATUS_SPP_OFFSET        8
#define MSTATUS_SPP_MASK          1L << MSTATUS_SPP_OFFSET
#define MSTATUS_MPP_OFFSET        11UL
#define MSTATUS_MPP_MASK          3UL << MSTATUS_MPP_OFFSET

#define RV32_SATP_MODE_OFFSET     30
#define RV64_SATP_MODE_OFFSET     60
#define SATP_BARE_MODE            0
#define SATP_SV32_MODE            1
#define SATP_SV39_MODE            8
#define SATP_SV48_MODE            9

#define SIE_SSIE_OFFSET           1
#define SIE_SSIE_MASK             1L << SIE_SSIE_OFFSET
#define SIE_STIE_OFFSET           5
#define SIE_STIE_MASK             1L << SIE_STIE_OFFSET
#define SIE_SEIE_OFFSET           9
#define SIE_SEIE_MASK             1 << SIE_SEIE_OFFSET

#endif /* __DEFS_H__ */