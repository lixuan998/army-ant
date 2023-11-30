include common.mk

BSP_DIR := bsp

OBJS += ${BSP_DIR}/obj/boot.o
D += ${BSP_DIR}/obj/boot.d
OBJS += ${BSP_DIR}/obj/uart.o
D += ${BSP_DIR}/obj/uart.d
OBJS += ${BSP_DIR}/obj/main.o
D += ${BSP_DIR}/obj/main.d
OBJS += ${BSP_DIR}/obj/startup.o
D += ${BSP_DIR}/obj/startup.d



all : ${OBJS} ${BSP_DIR}/boot_config.lds

${BSP_DIR}/obj/boot.o : ${BSP_DIR}/boot.c
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/obj/uart.o : ${BSP_DIR}/driver/uart/src/uart.c
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/obj/main.o : ${BSP_DIR}/main.c
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/obj/startup.o : ${BSP_DIR}/startup.S
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/boot_config.lds : ${BSP_DIR}/boot_config.c
	@echo building boot_config.lds
	@$(CC) -E -P  $^ -o $@