include config.mk

OBJS += tools/uart_file_receiver/obj/uart_file_receiver.o
D += tools/uart_file_receiver/obj/uart_file_receiver.d
all : ${OBJS}

tools/uart_file_receiver/obj/uart_file_receiver.o : tools/uart_file_receiver/uart_file_recevier.c
	@$(CC) $(CFLAGS) -c $^ -o $@
