include config.mk
include lib/lib.mk
include arch/arch.mk
include ${BSP_DIR}/bsp.mk
include kernel/kernel.mk
include console/console.mk
include fs/fs.mk
include tools/uart_file_receiver/uart_file_receiver.mk


all : army-ant.bin army-ant.elf

army-ant.bin : army-ant.elf
	@echo "Making binary..."
	@${OBJCOPY} army-ant.elf -O binary army-ant.bin	

army-ant.elf : ${OBJS}
	@echo "Linking ELF..."
	@${LD} -z max-page-size=4096 -L${BSP_DIR} -T ${BSP_DIR}/linkld.ld $^ -o $@
	@echo "Making objdump to army-ant.txt..."
	@${OBJDUMP} -d army-ant.elf > army-ant.txt

.PHONY : clean mkdir
clean : 
	@echo "Cleaning..."
	@rm -f *.o *.elf *.d *.bin army-ant.txt *.lds ${OBJS} ${D} ${BSP_DIR}/*.lds
mkdir :
	@echo "Making obj directories..."
	@mkdir -p arch/${ARCH}/obj
	@mkdir -p ${BSP_DIR}/obj
	@mkdir -p kernel/obj
	@mkdir -p console/obj
	@mkdir -p driver/obj
	@mkdir -p lib/obj
	@mkdir -p tools/uart_file_receiver/obj
	@mkdir -p fs/obj
	@echo "Obj directories created."