rm *.o 
rm *.out
rm *.bin
/opt/riscv/bin/riscv64-unknown-linux-gnu-gcc -Wall -Werror -O -fno-omit-frame-pointer -ggdb -gdwarf-2 -MD -mcmodel=medany -ffreestanding -fno-common -nostdlib -mno-relax -I. -I.. -fno-stack-protector -fno-pie -no-pie -march=rv64g -nostdinc -I. -Ikernel -c assemble_test.S -o assemble_test.o
/opt/riscv/bin/riscv64-unknown-linux-gnu-gcc -Wall -Werror -O -fno-omit-frame-pointer -ggdb -gdwarf-2 -MD -mcmodel=medany -ffreestanding -fno-common -nostdlib -mno-relax -I. -I.. -fno-stack-protector -fno-pie -no-pie -march=rv64g -nostdinc -I. -Ikernel -c ../arch/riscv/src/syscall_interface.S -o syscall_interface.o
/opt/riscv/bin/riscv64-unknown-linux-gnu-ld -z max-page-size=4096 -N -e start -Ttext 0 -o assemble_test.out assemble_test.o syscall_interface.o
/opt/riscv/bin/riscv64-unknown-linux-gnu-objdump -d assemble_test.out > assemble_test_output.txt
/opt/riscv/bin/riscv64-unknown-linux-gnu-objcopy assemble_test.out -S -O binary assemble_test.bin
od -t xC assemble_test.bin