army-ant.elf : startup.o main.o
	@arm-none-eabi-ld -T linkld.ld $^ -o $@

startup.o : startup.S
	@arm-none-eabi-gcc -c startup.S -o startup.o

main.o : main.c
	@arm-none-eabi-gcc -c main.c -o main.o

.PHONY : clean
clean : 
	@rm -f *.o *.elf
