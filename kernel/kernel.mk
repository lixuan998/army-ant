include common.mk

OBJS += kernel/obj/k_spinlock.o
D += kernel/obj/k_spinlock.d
OBJS += kernel/obj/k_stdio.o
D += kernel/obj/k_stdio.d

all : ${OBJS}

kernel/obj/k_spinlock.o : kernel/src/k_spinlock.c
	@$(CC) $(CFLAGS) -c $^ -o $@
kernel/obj/k_stdio.o : kernel/src/k_stdio.c
	@$(CC) $(CFLAGS) -c $^ -o $@
