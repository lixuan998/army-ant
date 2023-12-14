include config.mk

OBJS += kernel/obj/k_spinlock.o
D += kernel/obj/k_spinlock.d
OBJS += kernel/obj/k_stdio.o
D += kernel/obj/k_stdio.d
OBJS += kernel/obj/k_vm.o
D += kernel/obj/k_vm.d
OBJS += kernel/obj/k_paging.o
D += kernel/obj/k_paging.d
OBJS += kernel/obj/k_stdlib.o
D += kernel/obj/k_stdlib.d
OBJS += kernel/obj/k_interrupt.o
D += kernel/obj/k_interrupt.d
OBJS += kernel/obj/k_int_vector.o
D += kernel/obj/k_int_vector.d
OBJS += kernel/obj/k_time.o
D += kernel/obj/k_time.d
OBJS += kernel/obj/k_string.o
D += kernel/obj/k_string.d

all : ${OBJS}

kernel/obj/k_spinlock.o : kernel/src/k_spinlock.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_stdio.o : kernel/src/k_stdio.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_vm.o : kernel/src/k_vm.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_paging.o : kernel/src/k_paging.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_stdlib.o : kernel/src/k_stdlib.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_interrupt.o : kernel/src/k_interrupt.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_int_vector.o : kernel/k_int_vector.S
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_time.o : kernel/src/k_time.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_string.o : kernel/src/k_string.c
	@$(CC) $(CFLAGS) -c $^ -o $@