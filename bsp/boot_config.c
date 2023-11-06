#define BOOT_ROM_BASE_ADDR  0x00000000
#define BOOT_ROM_SIZE       (4 * 1024 * 1024)

#define BOOT_RAM_BASE_ADDR  0x40000000
#define BOOT_RAM_SIZE       (1 * 1024 * 1024 * 1024)

#define BOOT_TEXT_SEG_SIZE  (16 * 1024 * 1024)
#define BOOT_DATA_SEG_SIZE  (16 * 1024 * 1024)
#define BOOT_DMA_SIZE       (16 * 1024 * 1024)
#define BOOT_APP_SIZE       (16 * 1024 * 1024)

#define INIT_STACK_SIZE     (128 * 1024)

MEMORY
{
    TEXT_SEG (rx) : ORIGIN = BOOT_RAM_BASE_ADDR, LENGTH = BOOT_TEXT_SEG_SIZE
    DATA_SEG (rw) : ORIGIN = (BOOT_RAM_BASE_ADDR + BOOT_TEXT_SEG_SIZE), LENGTH = BOOT_DATA_SEG_SIZE
}   

BOOT_STACK_SIZE = INIT_STACK_SIZE;