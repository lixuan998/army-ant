include config.mk

# OBJS += arch/${ARCH}/obj/riscv_vm.o
# D += arch/${ARCH}/obj/riscv_vm.d
# OBJS += arch/${ARCH}/obj/riscv_spinlock.o
# D += arch/${ARCH}/obj/riscv_spinlock.d


all : ${OBJS}

# arch/${ARCH}/obj/riscv_vm.o : arch/${ARCH}/src/riscv_vm.c
# 	@$(CC) $(CFLAGS) -c $^ -o $@

# arch/${ARCH}/obj/riscv_spinlock.o : arch/${ARCH}/src/riscv_spinlock.c
# 	@$(CC) $(CFLAGS) -c $^ -o $@