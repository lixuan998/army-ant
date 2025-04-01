include config.mk

Q=

AA_ARCH_PATH := arch/$(AA_ARCH)
AA_ARCH_DIRS := mm sched

AA_BOARD_PATH = arch/$(AA_ARCH)/plat/$(AA_BOARD)
AA_BOARD_DIRS = timer uart plic

INCLUDE_DIR := -Iinclude -Iinclude/arch/${AA_ARCH} \
			   -Iinclude/arch/${AA_ARCH}/plat/${AA_BOARD}
DIRS := lib mm lock irq \
        $(AA_ARCH_PATH) $(addprefix $(AA_ARCH_PATH)/,$(AA_ARCH_DIRS)) \
		$(AA_BOARD_PATH) $(addprefix $(AA_BOARD_PATH)/,$(AA_BOARD_DIRS))


C_SRCS := $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
C_OBJS := $(C_SRCS:%.c=%.c.o)

ASM_SRCS := $(foreach dir,$(DIRS),$(wildcard $(dir)/*.S))
ASM_OBJS := $(ASM_SRCS:%.S=%.S.o)

INIT_C_SRCS := $(wildcard $(AA_ARCH_PATH)/boot/plat/$(AA_BOARD)/init/*.c)
INIT_C_OBJS := $(INIT_C_SRCS:%.c=%.c.o)

INIT_ASM_SRCS := $(wildcard $(AA_ARCH_PATH)/boot/plat/$(AA_BOARD)/init/*.S)
INIT_ASM_OBJS := $(INIT_ASM_SRCS:%.S=%.S.o)

ALL_OBJS := $(C_OBJS) $(ASM_OBJS) $(INIT_C_OBJS) $(INIT_ASM_OBJS)
ALL_DEPS := $(ALL_OBJS:.o=.d)

all : army-ant.bin army-ant.elf


army-ant.bin : army-ant.elf
	@echo "Making binary..."
	@${OBJCOPY} army-ant.elf -O binary army-ant.bin	

army-ant.elf : ${ALL_OBJS}
	@echo "Linking ELF..."
	@${LD} ${LDFLAGS} -T $(AA_ARCH_PATH)/boot/plat/$(AA_BOARD)/init/$(LINKLD) $^ -o $@
	@echo "Making objdump to army-ant.txt..."
	@${OBJDUMP} -d army-ant.elf > army-ant.txt

# -include $(ALL_DEPS)

$(C_OBJS) $(INIT_C_OBJS): %.o: %
	$(Q)$(CC) $(INCLUDE_DIR) $(CFLAGS) -c $< -o $@

$(ASM_OBJS) $(INIT_ASM_OBJS): %.o: %
	$(Q)$(CC) $(INCLUDE_DIR) $(CFLAGS) -c $< -o $@

.PHONY : clean mkdir
clean : 
	@echo "Cleaning..."
	@rm -f $(ALL_OBJS) $(ALL_DEPS) army-ant.bin army-ant.elf army-ant.txt
	
CPUS := 1 
QEMU := qemu-system-${AA_ARCH}
QEMUOPTS = -machine virt -bios none -kernel army-ant.elf -m 128M -smp $(CPUS) -nographic
# QEMUOPTS += -global virtio-mmio.force-legacy=false
# QEMUOPTS += -drive file=fs.img,if=none,format=raw,id=x0
# QEMUOPTS += -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0

qemu : army-ant.elf 
	@echo "Running..."
	@$(QEMU) $(QEMUOPTS)
