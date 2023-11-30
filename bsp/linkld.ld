INCLUDE "boot_config.lds"

OUTPUT_ARCH(riscv) 

ENTRY(_start)

SECTIONS
{
    .text :
    {
        *(.text)
        *(.text.*)
        *(.romrun)
        *(.rodata)
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
    } > TEXT_SEG
    .data ORIGIN(DATA_SEG) : 
    {
        . = ALIGN(8);
        PROVIDE (_data_start = .);
        
        *(.data)
         
        . = ALIGN(8);
        PROVIDE (_data_end = .);
    } > DATA_SEG

    .bss (NOLOAD) :
    {
        . = ALIGN(8);
        *(.noinit)
        
        . = ALIGN(8);
        PROVIDE (_bss_start = .);
        
        *(.bss)
        
        . = ALIGN(8);
        *(COMMON)
        
        . = ALIGN(8);
        PROVIDE (_bss_end = .);
    } > DATA_SEG
}