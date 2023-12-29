include config.mk

OBJS += arch/${ARCH}/obj/riscv_cpu.o
D += arch/${ARCH}/obj/riscv_cpu.d


OBJS += arch/${ARCH}/obj/interrupt_vector.o
D += arch/${ARCH}/obj/interrupt_vector.d



all : ${OBJS}

# arch/${ARCH}/obj/riscv_vm.o : arch/${ARCH}/src/riscv_vm.c
# 	@$(CC) $(CFLAGS) -c $^ -o $@

arch/${ARCH}/obj/riscv_cpu.o : arch/${ARCH}/src/riscv_cpu.c
	@$(CC) $(CFLAGS) -c $^ -o $@
arch/${ARCH}/obj/interrupt_vector.o : arch/${ARCH}/src/interrupt_vector.S
	@$(CC) $(CFLAGS) -c $^ -o $@