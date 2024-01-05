include config.mk

OBJS += lib/obj/stdio.o
D += lib/obj/stdio.d
OBJS += lib/obj/stdlib.o
D += lib/obj/stdlib.d
OBJS += lib/obj/time.o
D += lib/obj/time.d
OBJS += lib/obj/string.o
D += lib/obj/string.d
OBJS += lib/obj/adt.o
D += lib/obj/adt.d

all : ${OBJS}

lib/obj/stdio.o : lib/src/stdio.c
	@$(CC) $(CFLAGS) -c $^ -o $@
lib/obj/stdlib.o : lib/src/stdlib.c
	@$(CC) $(CFLAGS) -c $^ -o $@
lib/obj/time.o : lib/src/time.c
	@$(CC) $(CFLAGS) -c $^ -o $@
lib/obj/string.o : lib/src/string.c
	@$(CC) $(CFLAGS) -c $^ -o $@
lib/obj/adt.o : lib/src/adt.c
	@$(CC) $(CFLAGS) -c $^ -o $@