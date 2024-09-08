// #include "bsp/driver/uart/uart.h"
// #include "uart_file_recevier.h"
// #include "lib/include/stdlib.h"
// #include "arch/riscv/include/riscv_mem_defs.h"
// #include "fs/include/ff.h"
// #include "arch/riscv/include/riscv_interrupt_defs.h"

// int file_parse(char *file, int size);

// /*
// file format:
//     5A      01      xx xx xx xx         ...         ...       5A 02
//     |  head  |   length of file name   |file name|file data|tail
// 0:success
// */
// int file_receiver()
// {
//     interrupt_disable();
//     char file[10000];
//     int size = 0;
//     int byte;

//     while (1)
//     {
//         if (size > 1 && 0x5A == file[size - 2] && 0x02 == file[size - 1])
//         {
//             break;
//         }

//         byte = uart_scan_char();

//         if (byte == -1)
//         {
//             continue;
//         }
//         file[size++] = byte;
//     }
//     interrupt_enable();
//     return file_parse(file, size);
// }

// int file_parse(char *file, int size)
// {
//     uint32 file_name_len;
//     int ret,i=0;

//     file += 2; // skip head
//     memcpy(&file_name_len, file, sizeof(file_name_len));
//     file += sizeof(file_name_len);

//     char file_name[file_name_len];
//     memcpy(file_name, file, file_name_len);
//     file += file_name_len;

//     int data_size = size - HEAD_SIZE - TAIL_SIZE - file_name_len - sizeof(file_name_len);
//     char file_data[data_size];
//     memcpy(file_data, file, data_size);

//     FIL fd;

//     ret = f_open(&fd, file_name, FA_CREATE_NEW | FA_WRITE);
//     if (ret != 0)
//     {
//         return ret;
//     }
//     while (data_size--)
//     {
//         ret = f_write(&fd, file_data[i], 1, NULL);

//         if(file_data[i] == 0x5B ){
//             if(file_data[i+1]==0x01){
//                 ret = f_write(&fd, 0x5A, 1, NULL);
//             }
//             else if(file_data[i+1]==0x02){
//                 ret=f_write(&fd,0x5B,1,NULL);
//             }
//         }
//         ++i;
//     }
//     ret = f_close(&fd);
//     if (ret != 0)
//     {
//         return ret;
//     }

//     return 0;
// }