TOOLCHAIN_DIR := /opt/riscv/bin

LINKLD := linkld.ld

CC := ${TOOLCHAIN_DIR}/riscv64-unknown-linux-gnu-gcc
LD := ${TOOLCHAIN_DIR}/riscv64-unknown-linux-gnu-ld
OBJCOPY := ${TOOLCHAIN_DIR}/riscv64-unknown-linux-gnu-objcopy
OBJDUMP := ${TOOLCHAIN_DIR}/riscv64-unknown-linux-gnu-objdump

CFLAGS = -Wall -O -fno-omit-frame-pointer -ggdb -gdwarf-2
CFLAGS += -MD
CFLAGS += -mcmodel=medany
CFLAGS += -ffreestanding -fno-common -nostdlib -mno-relax
CFLAGS += -I.
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)
CFLAGS += -march=rv64g -nostdinc