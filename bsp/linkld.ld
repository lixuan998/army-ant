OUTPUT_ARCH( "riscv" )
ENTRY(_start)

/* MEMORY
{
    TEXT_SEG (rx) : ORIGIN = 0x80000000, LENGTH = (100 * 1024)
    DATA_SEG (rw) : ORIGIN = (0x80000000 + (100 * 1024)), LENGTH = (500 * 1024)
} */

MEMORY
{
    TEXT_SEG (rx) : ORIGIN = 0x40000000, LENGTH = (100 * 1024)
    DATA_SEG (rw) : ORIGIN = (0x40000000 + (100 * 1024)), LENGTH = (1024 * 1024 * 1024)
}

SECTIONS
{
  . = 0x40000000;
  PROVIDE(kernel_start = .);
    
  .text : {
    bsp/obj/startup.o(.text .text.*)
    *(.text .text.*)
    /* . = ALIGN(0x1000);
    _trampoline = .; */
    /* *(trampsec) */
    . = ALIGN(0x1000);
    /* ASSERT(. - _trampoline == 0x1000, "error: trampoline larger than one page"); */
    PROVIDE(kernel_text_end = .);
  } > TEXT_SEG

  .rodata : {
    . = ALIGN(16);
    *(.srodata .srodata.*) /* do not need to distinguish this from .rodata */
    . = ALIGN(16);
    *(.rodata .rodata.*)
  } > DATA_SEG

  .data : {
    . = ALIGN(16);
    *(.sdata .sdata.*) /* do not need to distinguish this from .data */
    . = ALIGN(16);
    *(.data .data.*)
  } > DATA_SEG

  .bss : {
    . = ALIGN(16);
    *(.sbss .sbss.*) /* do not need to distinguish this from .bss */
    . = ALIGN(16);
    *(.bss .bss.*)
  } > DATA_SEG

  . = ALIGN(0x1000);
  PROVIDE(kernel_end = .);
}
