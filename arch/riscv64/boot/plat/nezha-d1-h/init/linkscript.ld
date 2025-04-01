OUTPUT_ARCH( "riscv" )
ENTRY(_start)

MEMORY
{
    TEXT_SEG (rx) : ORIGIN = 0x40000000, LENGTH = (100 * 1024)
    DATA_SEG (rw) : ORIGIN = (0x40000000 + (100 * 1024)), LENGTH = (100 * 1024 * 1024)
}

SECTIONS
{
  . = 0x40000000;
  PROVIDE(__kernel_start = .);
    
  .text : {
	KEEP(*(.startup))
    *(.text .text.*);
    . = ALIGN(0x1000);
    PROVIDE(__text_start = .);
    PROVIDE(__trampoline_start = .);
    *(trampoline_section);
    . = ALIGN(0x1000);
    PROVIDE(__trampoline_end = .);
    ASSERT(__trampoline_end - __trampoline_start == 0x1000, "error: trampoline size is not 0x1000");
  } > TEXT_SEG

  .rodata : {
    . = ALIGN(16);
    *(.srodata .srodata.*) /* do not need to distinguish this from .rodata */
    . = ALIGN(16);
    *(.rodata .rodata.*)
    PROVIDE(rodata_end_addr = .);
  } > DATA_SEG

  .data : {
    . = ALIGN(16);
    *(.sdata .sdata.*) /* do not need to distinguish this from .data */
    . = ALIGN(16);
    *(.data .data.*)
    PROVIDE(data_end_addr = .);
  } > DATA_SEG

  .bss : {
    PROVIDE(__bss_start = .);
    . = ALIGN(16);
    *(.sbss .sbss.*) /* do not need to distinguish this from .bss */
    . = ALIGN(16);
    *(.bss .bss.*)
    PROVIDE(__bss_end = .);
  } > DATA_SEG

  . = ALIGN(0x1000);
  PROVIDE(__kernel_end = .);
}
