make clean && make
qemu-system-riscv64 -machine virt -bios none -kernel army-ant.bin -m 1024M -smp 1 -nographic -D ./log.txt -d cpu,mmu,in_asm