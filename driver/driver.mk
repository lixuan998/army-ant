include config.mk

OBJS += driver/obj/sd_physical_layer.o
D += driver/obj/sd_physical_layer.d

all : ${OBJS}

driver/obj/sd_physical_layer.o : driver/src/sd_physical_layer.c
	@$(CC) $(CFLAGS) -c $^ -o $@