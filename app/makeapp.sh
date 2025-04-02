rm *.o 
rm *.out
rm *.bin
riscv64-unknown-elf-gcc -Wall -Werror -O -fno-omit-frame-pointer -ggdb -gdwarf-2 -MD -mcmodel=medany -ffreestanding -fno-common -nostdlib -mno-relax -I. -I.. -fno-stack-protector -fno-pie -no-pie -march=rv64g -nostdinc -I. -Ikernel -c assemble_test.S -o assemble_test.o
riscv64-unknown-elf-gcc -Wall -Werror -O -fno-omit-frame-pointer -ggdb -gdwarf-2 -MD -mcmodel=medany -ffreestanding -fno-common -nostdlib -mno-relax -I. -I.. -fno-stack-protector -fno-pie -no-pie -march=rv64g -nostdinc -I. -Ikernel -c ../arch/riscv/src/syscall_interface.S -o syscall_interface.o
riscv64-unknown-elf-ld -z max-page-size=4096 -N -e start -Ttext 0 -o assemble_test.out assemble_test.o syscall_interface.o
riscv64-unknown-elf-objdump -d assemble_test.out > assemble_test_output.txt
riscv64-unknown-elf-objcopy assemble_test.out -S -O binary assemble_test.bin
od -t xC assemble_test.bin