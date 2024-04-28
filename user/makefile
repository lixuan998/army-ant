include ../config.mk

test.out : test.o
	${CC} -I .. test.o -o test.out

test.o : test.S
	@$(CC) -I .. $(CFLAGS) -c $^ -o $@