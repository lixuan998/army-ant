#ifndef __ASM_OPERATION_H__
#define __ASM_OPERATION_H__

/****************************************************************************************************************************************************
 * This file mainly defines some assembler operations embedded in C
****************************************************************************************************************************************************/

#include "../../../kernel/include/k_defs.h"

/****************************************************************************************************************************************************
 * 
 * 
 * mstatus Bitmap in RV64:
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |   Bit   | Abbr Name |              Full Name               |                    Description                      |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    0    |    WPRI   |           Write Privilege            |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    1    |    SIE    |      Supervisor Interrupt Enable     |      Turn on/off interrupt in Supervisor mode       |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    2    |    WPRI   |           Write Privilege            |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    3    |    MIE    |      Machine Interrupt Enable        |        Turn on/off interrupt in Machine mode        |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    4    |    WPRI   |            Write Privilege           |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    5    |    SPIE   | Supervisor Previous Interrupt Enable | Previously Turn on/off interrupt in Supervisor mode |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    6    |    UBE    |          User Mode Big-Endian        |             Use Big-Endian in User mode             |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+ 
 * |    7    |    MPIE   |   Machine Previous Interrupt Enable  |   Previously Turn on/off interrupt in Machine mode  |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    8    |    SPP    |    Supervisor Previous Privilege     |             Save previous privilege mode            |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |   9~10  |    VS     |           Virtual Supervisor         |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |  11~12  |    MPP    |      Machine Privious Privilege      |             Save previous privilege mode            |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |  13~14  |    FS     |         Floating-Point Status        |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |  15~16  |    XS     |           Extension Status           |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    17   |    MPRV   |           Modify Privilege           |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    18   |    SUM    |     Supervisor User Memory Access    |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    19   |    MXR    |        Make Excutable Readable       |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    20   |    TVM    |         Trap Virtual Memory          |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    21   |    TW     |            Timeout Wait              |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    22   |    TSR    |               Trap SRET              |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |  23~31  |    WPRI   |           Write Privilege            |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |  32~33  |    UXL    |      User-mode Extension Level       |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |  34~35  |    SXL    |    Supervisr-mode Extension Level    |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    36   |    SBE    |         Store Barrier Enable         |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    37   |    MBE    |       Machine Breakpoint Enable      |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |  38~62  |    WPRI   |           Write Privilege            |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * |    63   |     SD    |             State Dirty              |                       NONE                          |
 * +---------+-----------+--------------------------------------+-----------------------------------------------------+
 * 
 * 
****************************************************************************************************************************************************/

/****************************************************************************************************************************************************
 * @brief This function's purpose is to read the Machine Status Register, which  keeps track of and controls the hart’s current
 * operating state. This register is in the Machine Mode.
****************************************************************************************************************************************************/
uint64 inline r_mstatus()
{
  uint64 mstatus;
  asm volatile("csrr %0, mstatus" : "=r" (mstatus) );
  return mstatus;
}

/****************************************************************************************************************************************************
 * @brief this function's purpose is to read the Hart ID Register, which contains the integer ID of the hardware
 * thread running the code. This register is in the Machine Mode.
****************************************************************************************************************************************************/
uint64 inline r_mhartid()
{
  uint64 mhartid;
  asm volatile("csrr %0, mhartid" : "=r" (x) );
  return mhartid;
}

#endif  /* __ASM_OPERATION_H__ */