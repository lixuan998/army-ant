#ifndef UART_FILE_RECEIVER_H
#define UART_FILE_RECEIVER_H

#define HEAD_SIZE 2
#define TAIL_SIZE 2

extern char file_buffer[10000];
extern int file_buffer_idx;
extern int file_name_len;
extern int data_size;
extern int ret;

int file_receiver();
void file_byte_receive(char data);
int is_file_data_complete();
int file_parse(char *file, int size);

#endif /* UART_FILE_RECEIVER_H */