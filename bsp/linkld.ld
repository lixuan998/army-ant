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
  PROVIDE(kernel_start_addr = .);
    
  .text : {
    bsp/obj/startup.o(.text .text.*);
    *(.text .text.*);
    . = ALIGN(0x1000);
    PROVIDE(kernel_text_end_addr = .);
    PROVIDE(trampoline_start_addr = .);
    *(trampoline_section);
    . = ALIGN(0x1000);
    PROVIDE(trampoline_end_addr = .);
    ASSERT(trampoline_end_addr - trampoline_start_addr == 0x1000, "error: trampoline size is not 0x1000");
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
    PROVIDE(bss_start_addr = .);
    . = ALIGN(16);
    *(.sbss .sbss.*) /* do not need to distinguish this from .bss */
    . = ALIGN(16);
    *(.bss .bss.*)
    PROVIDE(bss_end_addr = .);
  } > DATA_SEG

  . = ALIGN(0x1000);
  PROVIDE(kernel_end_addr = .);
}