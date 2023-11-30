TOOLCHAIN_DIR=/usr/local/riscv/bin/

OUTPUT_DIR := ./build

SRC_DIR := ./arch/riscv/src	\
		   ./b	\

OBJS = \
	main.o	\
	startup.o	\
	boot.o	\
	uart.o

CFLAGS = -Wall -O -fno-omit-frame-pointer -ggdb -gdwarf-2
CFLAGS += -MD
CFLAGS += -mcmodel=medany
CFLAGS += -ffreestanding -fno-common -nostdlib -mno-relax
CFLAGS += -I.
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)

army-ant.bin : army-ant.elf
	${TOOLCHAIN_DIR}riscv64-unknown-linux-gnu-objcopy army-ant.elf -O binary army-ant.bin	

army-ant.elf : ${OBJS}
	@echo "Linking..."
	@${TOOLCHAIN_DIR}riscv64-unknown-linux-gnu-gcc -E -P boot_config.c -o boot_config.lds
	@${TOOLCHAIN_DIR}riscv64-unknown-linux-gnu-ld -z max-page-size=4096 -T linkld.ld $^ -o $@
	@${TOOLCHAIN_DIR}riscv64-unknown-linux-gnu-objdump -d army-ant.elf > army-ant.txt

startup.o : startup.S
	@${TOOLCHAIN_DIR}riscv64-unknown-linux-gnu-gcc $(CFLAGS) -march=rv64g -nostdinc -c startup.S -o startup.o

main.o : main.c
	@${TOOLCHAIN_DIR}riscv64-unknown-linux-gnu-gcc $(CFLAGS) -march=rv64g -nostdinc -c main.c -o main.o

boot.o : boot.c
	@${TOOLCHAIN_DIR}riscv64-unknown-linux-gnu-gcc $(CFLAGS) -march=rv64g -nostdinc -c boot.c -o boot.o
	


.PHONY : clean
clean : 
	@rm -f *.o *.elf *.d *.bin army-ant.txt *.lds
