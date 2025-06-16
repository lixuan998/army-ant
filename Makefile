include config.mk

Q=

AA_ARCH_PATH := arch/$(ARCH)
AA_ARCH_DIRS := mm sched irq

AA_PLAT_PATH := arch/$(ARCH)/plat/$(PLAT)
AA_PLAT_DIRS := timer uart plic

AA_TOOLS_PATH := tools
# AA_TOOLS_DIR := elf 

INCLUDE_DIR := -Iinclude -Iinclude/arch/${ARCH} \
			   -Iinclude/arch/${ARCH}/plat/${PLAT} \
			   -Itools/elf

DIRS := lib mm lock irq sched\
        $(AA_ARCH_PATH) $(addprefix $(AA_ARCH_PATH)/,$(AA_ARCH_DIRS)) \
		$(AA_PLAT_PATH) $(addprefix $(AA_PLAT_PATH)/,$(AA_PLAT_DIRS)) \
		$(AA_TOOLS_PATH) $(addprefix $(AA_TOOLS_PATH)/,$(AA_TOOLS_DIR))

C_SRCS := $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
C_OBJS := $(C_SRCS:%.c=%.c.o)

ASM_SRCS := $(foreach dir,$(DIRS),$(wildcard $(dir)/*.S))
ASM_OBJS := $(ASM_SRCS:%.S=%.S.o)

INIT_C_SRCS := $(wildcard $(AA_ARCH_PATH)/plat/$(PLAT)/boot/*.c)
INIT_C_OBJS := $(INIT_C_SRCS:%.c=%.c.o)

INIT_ASM_SRCS := $(wildcard $(AA_ARCH_PATH)/plat/$(PLAT)/boot/*.S)
INIT_ASM_OBJS := $(INIT_ASM_SRCS:%.S=%.S.o)

ALL_OBJS := $(C_OBJS) $(ASM_OBJS) $(INIT_C_OBJS) $(INIT_ASM_OBJS)
ALL_DEPS := $(ALL_OBJS:.o=.d)

all : army-ant.bin army-ant.elf


army-ant.bin : army-ant.elf
	@echo "Making binary..."
	@${OBJCOPY} army-ant.elf -O binary army-ant.bin	

army-ant.elf : ${ALL_OBJS}
	@echo "Linking ELF..."
	@${LD} ${LDFLAGS} -T $(AA_ARCH_PATH)/plat/$(PLAT)/boot/$(LINKLD) $^ -o $@
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
	
QEMU := qemu-system-${ARCH}
QEMUOPTS = -machine virt -bios none -kernel army-ant.elf -m 128M -smp $(CPU_NUM) -nographic
# QEMUOPTS += -global virtio-mmio.force-legacy=false
# QEMUOPTS += -drive file=fs.img,if=none,format=raw,id=x0
# QEMUOPTS += -device virtio-blk-device,drive=x0,bus=virtio-mmio-bus.0

qemu : army-ant.elf 
	@echo "Running..."
	@$(QEMU) $(QEMUOPTS)
