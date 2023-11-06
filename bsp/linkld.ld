INCLUDE "boot_config.lds"

OUTPUT_FORMAT(elf32-littlearm)
OUTPUT_ARCH(ARM) 

ENTRY(_start)

SECTIONS
{
    .text :
    {
        startup.o(.text)
        *(.text)
        *(.text.*)
        *(.romrun)
        *(.rodata)
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
    } > TEXT_SEG

    .got : 
    { 
        *(.got.plt) 
        *(.got) 
    } > TEXT_SEG

    .ARM.exidx :
    {
        . = ALIGN(4); 
        PROVIDE(__exidx_start = .); 
        
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        
        . = ALIGN(4); 
        PROVIDE(__exidx_end = .); 
    } > TEXT_SEG

    .ctors :
    {
        . = ALIGN(8);
        
        KEEP (*cppRtBegin*.o(.ctors))
        KEEP (*(.preinit_array))
        KEEP (*(.init_array))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
        KEEP (*cppRtEnd*.o(.ctors))
    } > TEXT_SEG
  
    .dtors :
    {
        . = ALIGN(8);
        
        KEEP (*cppRtBegin*.o(.dtors))
        KEEP (*(.fini_array))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
        KEEP (*cppRtEnd*.o(.dtors))
    } > TEXT_SEG

    . = ALIGN(8);
    PROVIDE (_text_end = .);

    .data ORIGIN(DATA_SEG) : AT (_text_end)
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

    .stack (NOLOAD) :
    {
        . = ALIGN(8);
        PROVIDE (_stack_top = .);
        
        . += BOOT_STACK_SIZE;
        
        . = ALIGN(8);
        /* PROVIDE (_stack_end = .); */
    } > DATA_SEG

    .heap (NOLOAD) :
    {
        . = ALIGN(8);
        PROVIDE (_heap_start = .);
        
        __heap_end = ORIGIN(DATA_SEG) + LENGTH(DATA_SEG) - 128;
        PROVIDE (_heap_end = .);
    } > DATA_SEG
}