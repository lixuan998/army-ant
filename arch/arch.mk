include config.mk

ARCH := riscv

OBJS += arch/${ARCH}/obj/riscv_vm.o
D += arch/${ARCH}/obj/riscv_vm.d
OBJS += arch/${ARCH}/obj/riscv_interrupt.o
D += arch/${ARCH}/obj/riscv_interrupt.d
OBJS += arch/${ARCH}/obj/riscv_cpu.o
D += arch/${ARCH}/obj/riscv_cpu.d
OBJS += arch/${ARCH}/obj/riscv_spinlock.o
D += arch/${ARCH}/obj/riscv_spinlock.d


all : ${OBJS}

arch/${ARCH}/obj/riscv_vm.o : arch/${ARCH}/src/riscv_vm.c
	@$(CC) $(CFLAGS) -c $^ -o $@

arch/${ARCH}/obj/riscv_interrupt.o : arch/${ARCH}/src/riscv_interrupt.c
	@$(CC) $(CFLAGS) -c $^ -o $@

arch/${ARCH}/obj/riscv_cpu.o : arch/${ARCH}/src/riscv_cpu.c
	@$(CC) $(CFLAGS) -c $^ -o $@

arch/${ARCH}/obj/riscv_spinlock.o : arch/${ARCH}/src/riscv_spinlock.c
	@$(CC) $(CFLAGS) -c $^ -o $@