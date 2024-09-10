#include "bsp/driver/uart/uart.h"
#include "uart_file_receiver.h"
#include "lib/include/stdlib.h"
#include "lib/include/stdio.h"
#include "arch/riscv/include/riscv_mem_defs.h"
#include "fs/include/ff.h"
#include "arch/riscv/include/riscv_interrupt_defs.h"

char file_buffer[10000];
int file_buffer_idx = 0;

/*
file format:
    5A      01      xx xx xx xx         ...         ...       5A 02
    |  head  |   length of file name   |file name|file data|tail
0:success
*/

void file_byte_receive(char data)
{
    if (file_buffer_idx == 0)
    {
        if (data == 0x5A)
            file_buffer[file_buffer_idx++] = data;
    }
    else if (file_buffer_idx == 1)
    {
        if (data == 0x01)
        {
            file_buffer[file_buffer_idx++] = data;
        }
        else
        {
            file_buffer_idx = 0;
        }
    }
    else if (file_buffer_idx > 1)
    {
        file_buffer[file_buffer_idx++] = data;
    }
    if (file_buffer_idx >= sizeof(file_buffer))
    {
        printf("file buffer overflow\n\r");
        file_buffer_idx = 0;
    }
    if (is_file_receive_complete())
    {
        file_parse(file_buffer, file_buffer_idx);
        file_buffer[file_buffer_idx] = '\0';
        file_buffer_idx = 0;
    }
}

int is_file_receive_complete()
{
    if (file_buffer_idx < 4)
        return 0;
    else
    {
        if (file_buffer[file_buffer_idx - 2] == 0x5A && file_buffer[file_buffer_idx - 1] == 0x02)
        {
            return 1;
        }
    }
}

int file_parse(char *file, int size)
{
    uint32 file_name_len = 0;
    int i = 0;

    file += HEAD_SIZE; // skip head
    for (int i = 0; i < sizeof(file_name_len); ++i)
    {
        file_name_len = (file_name_len << 8) | file[i];
    }
    file += sizeof(file_name_len);

    char file_name[file_name_len + 1];
    memcpy(file_name, file, file_name_len);
    file_name[file_name_len] = '\0';
    file += file_name_len;

    int file_data_size = size - HEAD_SIZE - TAIL_SIZE - file_name_len - sizeof(file_name_len);
    char file_data[file_data_size];
    memcpy(file_data, file, file_data_size);

    FIL fd;

    FRESULT ret = f_open(&fd, file_name, FA_CREATE_NEW | FA_WRITE);
    if (ret != 0)
    {
        return ret;
    }
    char ch;
    for (i = 0; i < file_data_size; ++i)
    {
        if (file_data[i] == 0x5B)
        {
            if (file_data[i + 1] == 0x01)
            {
                ch = 0x5A;
                ret = f_write(&fd, &ch, 1, NULL);
            }
            else if (file_data[i + 1] == 0x02)
            {
                ch = 0x5B;
                ret = f_write(&fd, &ch, 1, NULL);
            }
        }
        else
        {
            ret = f_write(&fd, &file_data[i], 1, NULL);
        }
    }
    ret = f_close(&fd);
    if (ret != 0)
    {
        return ret;
    }

    return 0;
}