include config.mk

OBJS += kernel/obj/k_spinlock.o
D += kernel/obj/k_spinlock.d
OBJS += kernel/obj/k_vm.o
D += kernel/obj/k_vm.d
OBJS += kernel/obj/k_paging.o
D += kernel/obj/k_paging.d
OBJS += kernel/obj/k_interrupt.o
D += kernel/obj/k_interrupt.d
OBJS += kernel/obj/k_proc.o
D += kernel/obj/k_proc.d
OBJS += kernel/obj/k_schedule.o
D += kernel/obj/k_schedule.d

all : ${OBJS}

kernel/obj/k_spinlock.o : kernel/src/k_spinlock.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_vm.o : kernel/src/k_vm.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_paging.o : kernel/src/k_paging.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_interrupt.o : kernel/src/k_interrupt.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_proc.o : kernel/src/k_proc.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_schedule.o : kernel/src/k_schedule.c
	@$(CC) $(CFLAGS) -c $^ -o $@