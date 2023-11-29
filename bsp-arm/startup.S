#include "../arch/assembler.h"


/****************************************************************************************************************************************************
 * +--------------+---------------+------------------------------+----------------+
 * |Processor Mode|Privilege Level|         Implemented          | Security state |
 * |--------------+---------------+------------------------------+----------------+
 * |   USR Mode   |      PL0      |            Always            |      Both      |
 * |--------------+---------------+------------------------------+----------------+
 * |   FIQ Mode   |      PL1      |            Always            |      Both      |
 * |--------------+---------------+------------------------------+----------------+
 * |   IRQ Mode   |      PL1      |            Always            |      Both      |
 * |--------------+---------------+------------------------------+----------------+
 * |   SVC Mode   |      PL1      |            Always            |      Both      |
 * |--------------+---------------+------------------------------+----------------+
 * |   MON Mode   |      PL1      |    With Security Extension   |   Secure Only  |
 * |--------------+---------------+------------------------------+----------------+
 * |   ABT Mode   |      PL1      |            Always            |      Both      |
 * |--------------+---------------+------------------------------+----------------+
 * |   HYP Mode   |      PL2      |With Virtualization Extensions| Non-Secure Only|
 * |--------------+---------------+------------------------------+----------------+
 * |   UND Mode   |      PL1      |            Always            |      Both      |
 * |--------------+---------------+------------------------------+----------------+
 * |   SYS Mode   |      PL1      |            Always            |      Both      |
 * +--------------+---------------+------------------------------+----------------+
 *
****************************************************************************************************************************************************/

#define USR_STACK_SIZE  0x00001000
#define FIQ_STACK_SIZE  0x00001000
#define IRQ_STACK_SIZE  0x00001000
#define SVC_STACK_SIZE  0x00001000
#define MON_STACK_SIZE  0x00001000
#define ABT_STACK_SIZE  0x00001000
#define HYP_STACK_SIZE  0x00001000
#define UND_STACK_SIZE  0x00001000
#define SYS_STACK_SIZE  0x00001000

#define STACK_SIZE      (USR_STACK_SIZE + \
                         FIQ_STACK_SIZE + \
                         IRQ_STACK_SIZE + \
                         SVC_STACK_SIZE + \
                         MON_STACK_SIZE + \
                         ABT_STACK_SIZE + \
                         HYP_STACK_SIZE + \
                         UND_STACK_SIZE + \
                         SYS_STACK_SIZE)

EXPORT_LABEL(_start)

IMPORT_LABEL(main)

SECTION(text)

FUNCTION(_start)
    ldr pc, _reset
    ldr pc, _undefined_instruction
    ldr pc, _software_interrupt
    ldr pc, _prefetch_abort
    ldr pc, _data_abort
    ldr pc, _irq_interrupt
    ldr pc, _fiq_interrupt
FUNCTION_END()

_reset:
    .word reset
_undefined_instruction:
    //.word undefined_instruction
_software_interrupt:
    //.word software_interrupt
_prefetch_abort:
    //.word prefetch_abort
_data_abort:
    //.word data_abort
_irq_interrupt:
    //.word irq_interrupt
_fiq_interrupt:
    //.word fiq_interrupt

reset:

/****************************************************************************************************************************************************
 * Initialize stack
****************************************************************************************************************************************************/
    ldr r0, =_stack_top

    // Switch to USR mode, and allocate stack space for USR mode.
    msr CPSR_c, #(CPSR_SET_USR32_MODE | CPSR_SET_I_MASK | CPSR_SET_F_MASK)
    mov sp, r0
    sub r0, #USR_STACK_SIZE

    // Switch to FIQ mode, and allocate stack space for FIQ mode.
    msr CPSR_c, #(CPSR_SET_FIQ32_MODE | CPSR_SET_I_MASK | CPSR_SET_F_MASK)
    mov sp, r0
    sub r0, #FIQ_STACK_SIZE

    // Switch to IRQ mode, and allocate stack space for IRQ mode.
    msr CPSR_c, #(CPSR_SET_IRQ32_MODE | CPSR_SET_I_MASK | CPSR_SET_F_MASK)
    mov sp, r0
    sub r0, #IRQ_STACK_SIZE

    // Switch to SVC mode, and allocate stack space for SVC mode.
    msr CPSR_c, #(CPSR_SET_SVC32_MODE | CPSR_SET_I_MASK | CPSR_SET_F_MASK)
    mov sp, r0
    sub r0, #SVC_STACK_SIZE

    // Switch to MON mode, and allocate stack space for MON mode.
    msr CPSR_c, #(CPSR_SET_MON32_MODE | CPSR_SET_I_MASK | CPSR_SET_F_MASK)
    mov sp, r0
    sub r0, #MON_STACK_SIZE

    // Switch to ABT mode, and allocate stack space for ABT mode.
    msr CPSR_c, #(CPSR_SET_ABT32_MODE | CPSR_SET_I_MASK | CPSR_SET_F_MASK)
    mov sp, r0
    sub r0, #ABT_STACK_SIZE

    // Switch to HYP mode, and allocate stack space for HYP mode.
    msr CPSR_c, #(CPSR_SET_HYP32_MODE | CPSR_SET_I_MASK | CPSR_SET_F_MASK)
    mov sp, r0
    sub r0, #HYP_STACK_SIZE

    // Switch to UND mode, and allocate stack space for UND mode.
    msr CPSR_c, #(CPSR_SET_UND32_MODE | CPSR_SET_I_MASK | CPSR_SET_F_MASK)
    mov sp, r0
    sub r0, #UND_STACK_SIZE

    // Switch to SYS mode, and allocate stack space for SYS mode.
    msr CPSR_c, #(CPSR_SET_SYS32_MODE | CPSR_SET_I_MASK | CPSR_SET_F_MASK)
    mov sp, r0
    sub r0, #SYS_STACK_SIZE

    // Switch to SVC mode.
    msr CPSR_c, #(CPSR_SET_SVC32_MODE | CPSR_SET_I_MASK | CPSR_SET_F_MASK)

/****************************************************************************************************************************************************
 * Initialize Data Segment
****************************************************************************************************************************************************/
    ldr r1, = _text_end
    ldr r2, = _data_start
    ldr r3, = _data_end
1:
    cmp r2, r3
    ldrlo r0, [r1], #4
    strlo r0, [r2], #4
    blo 1b

/****************************************************************************************************************************************************
 * Clear BSS Segment
****************************************************************************************************************************************************/
    mov r0, #0
    ldr r1, = _bss_start
    ldr r2, = _bss_end
2:
    cmp r1, r2
    strlo r0, [r1], #4
    blo 2b

    mov     r0 , #0
    mov     r1 , #0
    mov     r2 , #0
    mov     fp , #0

    ldr pc, =main

    b .

.end