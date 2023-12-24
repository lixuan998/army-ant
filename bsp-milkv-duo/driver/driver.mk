include config.mk

OBJS += ${BSP_DIR}/obj/uart.o
D += ${BSP_DIR}/obj/uart.d
OBJS += ${BSP_DIR}/obj/plic.o
D += ${BSP_DIR}/obj/plic.d
OBJS += ${BSP_DIR}/obj/timer.o
D += ${BSP_DIR}/obj/timer.d


all : ${OBJS}

${BSP_DIR}/obj/uart.o : ${BSP_DIR}/driver/uart/uart.c
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/obj/plic.o : ${BSP_DIR}/driver/plic/plic.c
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/obj/timer.o : ${BSP_DIR}/driver/timer/timer.c
	@$(CC) $(CFLAGS) -c $^ -o $@