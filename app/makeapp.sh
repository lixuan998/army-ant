rm *.o 
rm *.out
rm *.bin
/opt/riscv/bin/riscv64-unknown-linux-gnu-gcc -Wall -Werror -O -fno-omit-frame-pointer -ggdb -gdwarf-2 -MD -mcmodel=medany -ffreestanding -fno-common -nostdlib -mno-relax -I. -fno-stack-protector -fno-pie -no-pie -march=rv64g -nostdinc -I. -Ikernel -c test.c -o assemble_test.o
/opt/riscv/bin/riscv64-unknown-linux-gnu-ld -z max-page-size=4096 -N -e start -Ttext 0 -o assemble_test.out assemble_test.o
/opt/riscv/bin/riscv64-unknown-linux-gnu-objcopy assemble_test.out -S -O binary assemble_test.bin
od -t xC assemble_test.bin