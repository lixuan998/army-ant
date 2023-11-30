include common.mk
include arch/arch.mk
include bsp/bsp.mk
include kernel/kernel.mk

all : army-ant.bin army-ant.elf

army-ant.bin : army-ant.elf
	@echo "Making binary..."
	@${OBJCOPY} army-ant.elf -O binary army-ant.bin	

army-ant.elf : ${OBJS}
	@echo "Linking ELF..."
	@${LD} -z max-page-size=4096 -L${BSP_DIR} -T ${BSP_DIR}/linkld.ld $^ -o $@
	@echo "Making objdump to army-ant.txt..."
	@${OBJDUMP} -d army-ant.elf > army-ant.txt

.PHONY : clean
clean : 
	@echo "Cleaning..."
	@rm -f *.o *.elf *.d *.bin army-ant.txt *.lds ${OBJS} ${D} ${BSP_DIR}/*.lds