make clean && make -j
qemu-system-riscv64 -machine virt -bios none -kernel army-ant.bin -m 1024M -smp 1 -nographic -dtb sun20i-d1s-nezha-linux.dtb