include config.mk

OBJS += console/obj/console.o
D += console/obj/console.d

all : ${OBJS}

console/obj/console.o : console/src/console.c
	@$(CC) $(CFLAGS) -c $^ -o $@