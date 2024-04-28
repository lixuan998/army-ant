include config.mk

#uart folder
OBJS += ${BSP_DIR}/obj/uart.o
D += ${BSP_DIR}/obj/uart.d
#plic folder
OBJS += ${BSP_DIR}/obj/plic.o
D += ${BSP_DIR}/obj/plic.d
#timer folder
OBJS += ${BSP_DIR}/obj/timer.o
D += ${BSP_DIR}/obj/timer.d
#smhc folder
OBJS += ${BSP_DIR}/obj/sd.o
D += ${BSP_DIR}/obj/sd.d
#ccu folder
OBJS += ${BSP_DIR}/obj/ccu.o
D += ${BSP_DIR}/obj/ccu.d
#gpio folder
OBJS += ${BSP_DIR}/obj/gpio.o
D += ${BSP_DIR}/obj/gpio.d
#mmc folder
# OBJS += ${BSP_DIR}/obj/mmc_bsp.o
# D += ${BSP_DIR}/obj/mmc_bsp.d
# OBJS += ${BSP_DIR}/obj/mmc.o
# D += ${BSP_DIR}/obj/mmc.d

all : ${OBJS}

${BSP_DIR}/obj/uart.o : ${BSP_DIR}/driver/uart/uart.c
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/obj/plic.o : ${BSP_DIR}/driver/plic/plic.c
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/obj/timer.o : ${BSP_DIR}/driver/timer/timer.c
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/obj/sd.o : ${BSP_DIR}/driver/smhc/sd.c
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/obj/ccu.o : ${BSP_DIR}/driver/ccu/ccu.c
	@$(CC) $(CFLAGS) -c $^ -o $@

${BSP_DIR}/obj/gpio.o : ${BSP_DIR}/driver/gpio/gpio.c
	@$(CC) $(CFLAGS) -c $^ -o $@

# ${BSP_DIR}/obj/mmc_bsp.o : ${BSP_DIR}/driver/mmc/mmc_bsp.c
# 	@$(CC) $(CFLAGS) -c $^ -o $@
# ${BSP_DIR}/obj/mmc.o : ${BSP_DIR}/driver/mmc/mmc.c
# 	@$(CC) $(CFLAGS) -c $^ -o $@