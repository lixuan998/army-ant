include common.mk

OBJS += kernel/obj/k_spinlock.o
D += kernel/obj/k_spinlock.d
OBJS += kernel/obj/k_stdio.o
D += kernel/obj/k_stdio.d
OBJS += kernel/obj/k_cpu.o
D += kernel/obj/k_cpu.d
OBJS += kernel/obj/k_vm.o
D += kernel/obj/k_vm.d
OBJS += kernel/obj/k_paging.o
D += kernel/obj/k_paging.d
OBJS += kernel/obj/k_stdlib.o
D += kernel/obj/k_stdlib.d

all : ${OBJS}

kernel/obj/k_spinlock.o : kernel/src/k_spinlock.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_stdio.o : kernel/src/k_stdio.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_cpu.o : kernel/src/k_cpu.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_vm.o : kernel/src/k_vm.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_paging.o : kernel/src/k_paging.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_stdlib.o : kernel/src/k_stdlib.c
	@$(CC) $(CFLAGS) -c $^ -o $@