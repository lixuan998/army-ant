include common.mk

ARCH := riscv
OBJS += arch/${ARCH}/obj/vm.o
D += arch/${ARCH}/obj/vm.d


all : ${OBJS}

arch/${ARCH}/obj/vm.o : arch/${ARCH}/src/vm.c
	@$(CC) $(CFLAGS) -c $^ -o $@
