#include "bsp/driver/uart/uart.h"
#include "uart_file_receiver.h"
#include "lib/include/stdlib.h"
#include "lib/include/stdio.h"
#include "arch/riscv/include/riscv_mem_defs.h"
#include "fs/include/ff.h"
#include "arch/riscv/include/riscv_interrupt_defs.h"

char file_buffer[10000];
int file_buffer_idx = 0;
int data_size=0;
int ret=-1;

/*
file format:
    5A      01      xx xx xx xx         ...         ...       5A 02
    |  head  |   length of file name   |file name|file data|tail
0:success
*/
int file_receiver()
{
    interrupt_disable();
    char file[10000];
    int size = 0;
    int byte;

    while (1)
    {
        if (size > 1 && 0x5A == file[size - 2] && 0x02 == file[size - 1])
        {
            break;
        }

        byte = uart_scan_char();

        if (byte == -1)
        {
            continue;
        }
        file[size++] = byte;
    }
    interrupt_enable();
    return file_parse(file, size);
}

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
    if (is_file_data_complete())
    {
        file_parse(file_buffer, file_buffer_idx);
        file_buffer[file_buffer_idx] = '\0';
        file_buffer_idx = 0;
    }
}

int is_file_data_complete()
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
    uint32 file_name_len;
    int  i = 0;

    file += HEAD_SIZE; // skip head
    for (int i = 0; i < sizeof(file_name_len); ++i)
    {
        file_name_len = (file_name_len << 8) | file[i];
    }
    file += sizeof(file_name_len);

    char file_name[file_name_len];
    memcpy(file_name, file, file_name_len);
    file += file_name_len;

    data_size = size - HEAD_SIZE - TAIL_SIZE - file_name_len - sizeof(file_name_len);
    char file_data[data_size];
    memcpy(file_data, file, data_size);

    FIL fd;

    ret = f_open(&fd, "0:/makefile", FA_CREATE_NEW | FA_WRITE);
    if (ret != 0)
    {
        return ret;
    }
    char ch;
    // for(i = 0; i < data_size;++i)
    // {
        ch = file_data[i];
        ret = f_write(&fd, "hello", 5, NULL);
        
        // if (file_data[i] == 0x5B)
        // {
        //     if (file_data[i + 1] == 0x01)
        //     {
        //         ch = 0x5A;
        //         ret = f_write(&fd, &ch, 1, NULL);
        //     }
        //     else if (file_data[i + 1] == 0x02)
        //     {
        //         ch = 0x5B;
        //         ret = f_write(&fd, &ch, 1, NULL);
        //     }
        // }
    //     ++i;
    // }
    ret = f_close(&fd);
    if (ret != 0)
    {
        return ret;
    }

    return 0;
}