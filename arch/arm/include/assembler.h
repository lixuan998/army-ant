#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

/****************************************************************************************************************************************************
 * This file mainly defines some constants in arm architecture.
 * Also, it defines some operations in order to be compatible with various architectures.
****************************************************************************************************************************************************/

/****************************************************************************************************************************************************
 * Below are some defines about CPU mode, it is implemented by set the CPSR
 * register with 'msr' command.For better understanding, I'll give the bit map
 * about the CPSR register.
 * 
 * CSPR register:
 * +-------------+---+---+---+---+---+-------+---+---------------+---------------+-----------------------+---+---+---+---+---+-------------------+
 * |Bit:         | 31| 30| 29| 28| 27| 26~25 | 24|     23~20     |     19~16     |         15~10         | 9 | 8 | 7 | 6 | 5 |        4~0        |
 * |-------------+---+---+---+---+---+-------+---+---------------+---------------+-----------------------+---+---+---+---+---+-------------------+
 * |Symbol:      | N | Z | C | V | Q |IT[1:0]| J |    Reserved   |     GE[3:0]   |         IT[7:2]       | E | A | I | F | T |       M[4:0]      |
 * |-------------+---+---+---+---+---+-------+---+---------------+---------------+-----------------------+---+---+---+---+---+-------------------+
 * |Discription: |Condition flags|                                                                           | Mask bits |                       |
 * +-------------+---------------+---+-------+---------------------------------------------------------------+-----------+-----------------------+
 * 
 * Condition flags:
 *                  N: Negative condition flag.
 *                  Z: Zero condition flag.
 *                  C: Carry condition flag.
 *                  V: Overflow condition flag.
 * 
 * Q: Cumulative saturation bit.
 * 
 * IT[7:0]: If-Then execution state bits for the Thumb IT (If-Then) instruction.
 * 
 * J: Jazelle bit.
 * 
 * GE[3:0]:Greater than or Equal flags.
 * 
 * E: Endianness execution state bit.(0 for little-endian, 1 for big-endian)
 * 
 * Mask bits(1 for masked, 0 for not):
 *                  A: Asynchronous abort mask bit.
 *                  I: IRQ mask bit.
 *                  F: FRQ mask bit.
 * T: Thumb execution state bit.(This bit and the J execution state bit, determine the instruction set
 *                              state of the processor, ARM, Thumb, Jazelle, or ThumbEE.)
 * 
 * M[4:0]: Mode field. (This field determines the current mode of the processor.)
 * 
 * PS: Combinations of J and T
 * 
 * +---+---+-----------------------+
 * | J | T | Instruction set state |
 * +---+---+-----------------------+
 * | 0 | 0 |          ARM          |
 * +---+---+-----------------------+
 * | 0 | 1 |         Thumb         |
 * +---+---+-----------------------+
 * | 1 | 0 |        Jazelle        |
 * +---+---+-----------------------+
 * | 1 | 1 |        ThumbEE        |
 * +---+---+-----------------------+
****************************************************************************************************************************************************/

#define CPSR_SET_USR32_MODE     0x10    //  10000 in binary
#define CPSR_SET_FIQ32_MODE     0x11    //  10001 in binary
#define CPSR_SET_IRQ32_MODE     0x12    //  10010 in binary
#define CPSR_SET_SVC32_MODE     0x13    //  10011 in binary
#define CPSR_SET_MON32_MODE     0x14    //  10110 in binary
#define CPSR_SET_ABT32_MODE     0x15    //  10111 in binary
#define CPSR_SET_HYP32_MODE     0x16    //  11010 in binary
#define CPSR_SET_UND32_MODE     0x17    //  11011 in binary
#define CPSR_SET_SYS32_MODE     0x18    //  11111 in binary

#define CPSR_SET_A_MASK         0x100   //  100000000 in binary
#define CPSR_SET_I_MASK         0x80    //  10000000 in binary
#define CPSR_SET_F_MASK         0x40    //  1000000 in binary

/****************************************************************************************************************************************************
 * Below are some operations
****************************************************************************************************************************************************/

#define USE_ARM_INSTRUCTION_SET

#ifdef __GNUC__

    #define IMPORT_LABEL(label)         \
            .extern label
        
    #define EXPORT_LABEL(label)         \
            .global label

    #ifdef USE_ARM_INSTRUCTION_SET

        #define FUNCTION(func_label)    \
                .code   32;             \
                .balign 8;              \
                .type func, %function;  \
                func_label:
    #endif  /* USE_ARM_INSTRUCTION_SET */   

    #ifdef USE_THUMB_INSTRUCTION_SET
        #define FUNCTION(func_label)    \
                .code   16;             \
                .thumb;                 \
                .balign 8;              \
                .type func, %function;  \
                .syntax unified;        \
                func_label:
    #endif  /* USE_THUMB_INSTRUCTION_SET */

    #define FUNCTION_END()              \
                .ltorg

    #define TEXT_SEG(text_label)        \
                .text                   \
                .balign 8
    #define TEXT_SEG_END()              \
                .end
    
    #define SECTION(sec_label)          \
                .section .sec_label      
    
    #define WEAK(label)                 \
                .weak label
#else

    #define FUNCTION(func_label)        \
                func_label:

    #define FUNCTION_END()              

    #define SECTION(sec_label)          \
                area sec_label

#endif /* __GNUC__ */

/****************************************************************************************************************************************************
 * Frame pointer operations
****************************************************************************************************************************************************/

#define PUSH_FRAME()                    \
                push {fp, lr}           \
                add fp, sp, #4         

#define POP_FRAME()                     \
                pop  {fp, lr}

/****************************************************************************************************************************************************
 * Memory barrier, suitable for arm versions later than armv7.
****************************************************************************************************************************************************/

#define ARM_ISB()     ISB
#define ARM_DSB()     DSB
#define ARM_DMB()     DMB

#endif  /* __ASSEMBLER_H__ */