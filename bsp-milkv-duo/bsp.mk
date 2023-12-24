include config.mk
BSP_DIR := bsp-milkv-duo
include ${BSP_DIR}/driver/driver.mk

OBJS += ${BSP_DIR}/obj/boot.o
D += ${BSP_DIR}/obj/boot.d
OBJS += ${BSP_DIR}/obj/main.o
D += ${BSP_DIR}/obj/main.d
OBJS += ${BSP_DIR}/obj/startup.o
D += ${BSP_DIR}/obj/startup.d

all : ${OBJS}

${BSP_DIR}/obj/boot.o : ${BSP_DIR}/boot.c
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/obj/main.o : ${BSP_DIR}/main.c
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/obj/startup.o : ${BSP_DIR}/startup.S
	@$(CC) $(CFLAGS) -c $^ -o $@