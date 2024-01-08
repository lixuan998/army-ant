#include "arch/defs.h"
#include "lib/include/stdio.h"
#include "lib/include/time.h"
#include "lib/include/time.h"
#include "driver/uart/uart.h"
#include "driver/timer/timer.h"
#include "console/include/console.h"

void kernel_mem_init();
void kernel_vm_init();
void kernel_interrupt_init();

void general_spinlock_init();

void kernel_init();
void peripheral_init();

extern char kernel_stack[4096 * CPU_NUM];

int main()
{        
    printf("sizeof MEM_BLK: %d\n\r", sizeof(MEM_BLK));
    printf("kernel start: %x\n\r", (addr_t)kernel_start);
    printf("kernel end: %x\n\r", (addr_t)kernel_end);
    printf("kernel text end: %x\n\r", (addr_t)kernel_text_end);
    printf("kernel stack: %x\n\r", (addr_t)kernel_stack);
    printf("kernel rodata_end: %x\n\r", (addr_t)rodata_end);
    printf("kernel data_end: %x\n\r", (addr_t)data_end);
    printf("kernel bss_start: %x\n\r", bss_start);
    printf("kernel bss_end: %x\n\r", bss_end);
    printf("ram size: %x\n\r", RAM_SIZE);
    printf("ram top: %x\n\r", (addr_t)RAM_TOP);
    // printf("kernel memory paging initializing...\n\r");
    
    // printf("kernel memory paging initialized!\n\r");
    // printf("initializing virtual memory...\n\r");
    
    // printf("virtual memory initialized!\n\r");
    // printf("initializing interrupt...\n\r");
    
    
    // printf("interrupt initialized!\n\r");
    // printf("AAA\n\r");
    general_spinlock_init();
    kernel_init();
    // printf("BBB\n\r");
    peripheral_init();
    // printf("CCC\n\r");
    // console_init();
    char *ptr1;
    int *ptr2;
    double *ptr3;
    MEM_BLK *ptr4;
    MEM_BLK *ptr5;
    char *ptr6;
    int *ptr7;
    double *ptr8;
    for(int i = 0; i < 4080; ++ i)
    {
        ptr1 = (char *)malloc(sizeof(char));
        printf("ptr1: %x\n\r", ptr1);
        ptr2 = (int *)malloc(sizeof(int));
        printf("ptr2: %x\n\r", ptr2);
        ptr3 = (double *)malloc(sizeof(double));
        printf("ptr3: %x\n\r", ptr3);
        ptr4 = (MEM_BLK *)malloc(sizeof(MEM_BLK));
        printf("ptr4: %x\n\r", ptr4);
        ptr5 = (MEM_BLK *)malloc(sizeof(MEM_BLK));
        printf("ptr5: %x\n\r", ptr5);
        ptr6 = (char *)malloc(sizeof(char));
        printf("ptr6: %x\n\r", ptr6);
        ptr7 = (int *)malloc(sizeof(int));
        printf("ptr7: %x\n\r", ptr7);
        ptr8 = (double *)malloc(sizeof(double));
        printf("ptr8: %x\n\r", ptr8);

        sleep(5);

        free(ptr1);
        free(ptr2);
        free(ptr3);
        free(ptr4);
        free(ptr5);
        free(ptr6);
        free(ptr7);
        free(ptr8);
        sleep(5);
    }

    

    
    // for(int i = 0; i < 1025; ++ i)
    // {
    //     int *t;
    //     printf("t before: %x\n\r", t);
    //     t = (int *)malloc(sizeof(int));
    //     printf("t after: %x value: %d\n\r", t, (*t));
    // }
    

    while(1)
    {
        // uint32 duration = 0;
        // start_timing();
        // sleep(1);
        // stop_timing(&duration);
        // printf("duration: %d\n\r", duration);
        
    }
    return 0;
}

void kernel_mem_init()
{
    mem_paging_init();
}

void kernel_vm_init()
{
    extern pagetable_t kernel_pagetable;
    VM_MAP_INFO vm_map_info[] = {
        {
            //Map for peripherals.
            .virt_addr_start = 0,
            .phys_addr_start = 0,
            .size = (addr_t)kernel_start,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A | VM_SO_STRONG_ORDER
        },
        {
            //Map for kernel text segment
            .virt_addr_start = (addr_t)kernel_start,
            .phys_addr_start = (addr_t)kernel_start,
            .size = (addr_t)kernel_text_end - (addr_t)kernel_start,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_X | PTE_PERMISSION_A | VM_C_CACHEABLE | VM_B_BUFFERABLE
        },
        {
            //Map for the rest of kernel and the rest of RAM
            .virt_addr_start = (addr_t)kernel_text_end,
            .phys_addr_start = (addr_t)kernel_text_end,
            .size = (addr_t)RAM_TOP - (addr_t)kernel_text_end,
            .permisson = PTE_PERMISSION_R | PTE_PERMISSION_W | PTE_PERMISSION_D | PTE_PERMISSION_A | VM_C_CACHEABLE | VM_B_BUFFERABLE
        }
    };
    kernel_pagetable = pagetable_create(vm_map_info, 3);
    set_vm_pagetable(kernel_pagetable);
}

void kernel_interrupt_init()
{
    interrupt_init(kernel_interrupt_vector);
    interrupt_enable();
}

void general_spinlock_init()
{
    for(int i = 0; i < GENERAL_SPINLOCK_NUM; ++ i)
    {
        spinlock_init(general_spinlock + i, "general_spinlock");
    }
}

void kernel_init()
{
    kernel_mem_init();
    kernel_vm_init();
    kernel_interrupt_init();
}

void peripheral_init()
{
    uart_init();
    timer_start(0, 1000);
}
