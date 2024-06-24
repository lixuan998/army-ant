include config.mk

OBJS += arch/${ARCH}/obj/riscv_cpu.o
D += arch/${ARCH}/obj/riscv_cpu.d


OBJS += arch/${ARCH}/obj/interrupt_vector.o
D += arch/${ARCH}/obj/interrupt_vector.d

OBJS += arch/${ARCH}/obj/context_switch.o
D += arch/${ARCH}/obj/context_switch.d

OBJS += arch/${ARCH}/obj/trampoline.o
D += arch/${ARCH}/obj/trampoline.d

all : ${OBJS}

# arch/${ARCH}/obj/riscv_vm.o : arch/${ARCH}/src/riscv_vm.c
# 	@$(CC) $(CFLAGS) -c $^ -o $@

arch/${ARCH}/obj/riscv_cpu.o : arch/${ARCH}/src/riscv_cpu.c
	@$(CC) $(CFLAGS) -c $^ -o $@
arch/${ARCH}/obj/interrupt_vector.o : arch/${ARCH}/src/interrupt_vector.S
	@$(CC) $(CFLAGS) -c $^ -o $@
arch/${ARCH}/obj/context_switch.o : arch/${ARCH}/src/context_switch.S
	@$(CC) $(CFLAGS) -c $^ -o $@
arch/${ARCH}/obj/trampoline.o : arch/${ARCH}/src/trampoline.S
	@$(CC) $(CFLAGS) -c $^ -o $@