include config.mk

OBJS += fs/obj/diskio.o
D += fs/obj/diskio.d
OBJS += fs/obj/ff.o
D += fs/obj/ff.d
OBJS += fs/obj/ffsystem.o
D += fs/obj/ffsystem.d
OBJS += fs/obj/ffunicode.o
D += fs/obj/ffunicode.d

all : ${OBJS}

fs/obj/diskio.o : fs/src/diskio.c
	@$(CC) $(CFLAGS) -c $^ -o $@
fs/obj/ff.o : fs/src/ff.c
	@$(CC) $(CFLAGS) -c $^ -o $@
fs/obj/ffsystem.o : fs/src/ffsystem.c
	@$(CC) $(CFLAGS) -c $^ -o $@
fs/obj/ffunicode.o : fs/src/ffunicode.c
	@$(CC) $(CFLAGS) -c $^ -o $@