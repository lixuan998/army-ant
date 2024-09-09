#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

int set_opt(int, int, int, char, int);
int uart_send(int fd, void *buf, int len);
int uart_recv_timeout(int uart_fd, void *buf, int len, int timeout_ms);
int read_file(char *file_name, char **buffer);
int main(int argc, char **argv)
{
    int fd, ret, i = 10;
    char *uart3 = "/dev/ttyUSB0";
    // char *buffer="hello,world!\n";
    char *file_name;
    if (argc > 1)
    {
        file_name = argv[1];
    }
    else
    {
        printf("please input file name\n");
        return -1;
    }
    printf("file_name = %s\n", file_name);
    int file_fd = open(file_name, O_RDONLY);
    if (file_fd < 0)
    {
        printf("open file %s failed\n", file_name);
        return -1;
    }
    char *file_buffer;
    int file_len = read_file(file_name, &file_buffer);

    if ((fd = open(uart3, O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
    {
        printf("open %s is failed", uart3);
    }
    else
    {
        printf("open %s is success\n", uart3);
        set_opt(fd, 115200, 8, 'N', 1);

        char buffer[100000];
        int len = 0;
        buffer[0] = 0x5A;
        buffer[1] = 0x01;
        len += 2;

        int file_name_len = strlen(file_name);

        for(int i = sizeof(file_name_len)-1;i >= 0;--i)
        {
            buffer[len++] = (file_name_len >> (8 * i)) & 0xFF;
        }

        for(int i = 0;i < file_name_len;++i)
        {
            buffer[len++] = file_name[i];
        }

        for (int i = 0; i < file_len; ++i)
        {
            if (file_buffer[i] == 0x5A)
            {
                buffer[len++]=0x5B;
                buffer[len++]=0x01;
            }
            else if (file_buffer[i] == 0x5B)
            {
                buffer[len++]=0x5B;
                buffer[len++]=0x02;
            }else{
                buffer[len++]=file_buffer[i];
            }

        }

        buffer[len++] = 0x5A;
        buffer[len++] = 0x02;
        for(int i=0;i<len;++i)
            printf("%02x ",buffer[i]);
        ret = uart_send(fd, buffer, len);
        if (ret < 0)
            printf("write failed\n");
        else
        {
            printf("wr_static is %d\n", ret);
        }
    }
    close(fd);
    return 0;
}

int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio, oldtio;
    /*获取原有串口配置*/
    if (tcgetattr(fd, &oldtio) != 0)
    {
        perror("SetupSerial 1");
        return -1;
    }
    memset(&newtio, 0, sizeof(newtio));
    /*CREAD 开启串行数据接收，CLOCAL并打开本地连接模式*/
    newtio.c_cflag |= CLOCAL | CREAD;

    /*设置数据位*/
    newtio.c_cflag &= ~CSIZE;
    switch (nBits)
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    }
    /* 设置奇偶校验位 */
    switch (nEvent)
    {
    case 'O':
        newtio.c_cflag |= PARENB;
        newtio.c_cflag |= PARODD;
        newtio.c_iflag |= (INPCK | ISTRIP);
        break;
    case 'E':
        newtio.c_iflag |= (INPCK | ISTRIP);
        newtio.c_cflag |= PARENB;
        newtio.c_cflag &= ~PARODD;
        break;
    case 'N':
        newtio.c_cflag &= ~PARENB;
        break;
    }
    /* 设置波特率 */
    switch (nSpeed)
    {
    case 2400:
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);
        break;
    case 4800:
        cfsetispeed(&newtio, B4800);
        cfsetospeed(&newtio, B4800);
        break;
    case 9600:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    case 115200:
        cfsetispeed(&newtio, B115200);
        cfsetospeed(&newtio, B115200);
        break;
    case 460800:
        cfsetispeed(&newtio, B460800);
        cfsetospeed(&newtio, B460800);
        break;
    default:
        cfsetispeed(&newtio, B9600);
        cfsetospeed(&newtio, B9600);
        break;
    }
    /*设置停止位*/
    if (nStop == 1)                /*设置停止位；若停止位为1，则清除CSTOPB，若停止位为2，则激活CSTOPB*/
        newtio.c_cflag &= ~CSTOPB; /*默认为一位停止位； */
    else if (nStop == 2)
        newtio.c_cflag |= CSTOPB;
    /*设置最少字符和等待时间，对于接收字符和等待时间没有特别的要求时*/
    newtio.c_cc[VTIME] = 0; /*非规范模式读取时的超时时间；*/
    newtio.c_cc[VMIN] = 0;  /*非规范模式读取时的最小字符数*/
    /*tcflush清空终端未完成的输入/输出请求及数据；TCIFLUSH表示清空正收到的数据，且不读取出来 */
    tcflush(fd, TCIFLUSH);
    if ((tcsetattr(fd, TCSANOW, &newtio)) != 0)
    {
        perror("com set error");
        return -1;
    }
    //  printf("set done!\n\r");
    return 0;
}

int uart_send(int fd, void *buf, int len)
{
    int ret = 0;
    int count = 0;

    tcflush(fd, TCIFLUSH);

    while (len > 0)
    {

        ret = write(fd, (char *)buf + count, len);
        if (ret < 1)
        {
            break;
        }
        count += ret;
        len = len - ret;
    }

    return count;
}

int uart_recv_timeout(int uart_fd, void *buf, int len, int timeout_ms)
{
    int ret;
    size_t rsum = 0;
    ret = 0;
    fd_set rset;
    struct timeval t;

    while (rsum < len)
    {
        t.tv_sec = timeout_ms / 1000;
        t.tv_usec = (timeout_ms - t.tv_sec * 1000) * 1000;
        FD_ZERO(&rset);
        FD_SET(uart_fd, &rset);
        ret = select(uart_fd + 1, &rset, NULL, NULL, &t);
        if (ret <= 0)
        {
            if (ret == 0)
            {
                // timeout
                return -1;
            }
            if (errno == EINTR)
            {
                // 信号中断
                continue;
            }
            return -errno;
        }
        else
        {
            ret = read(uart_fd, (char *)buf + rsum, len - rsum);
            if (ret < 0)
            {
                return ret;
            }
            else
            {
                rsum += ret;
            }
        }
    }

    return rsum;
}
int read_file(char *file_name, char **buffer)
{
    struct stat fileStat;
    int fd;
    ssize_t bytesRead;

    fd = open(file_name, O_RDONLY); // 以只读方式打开文件
    if (fd == -1)
    {
        perror("Error opening file");
        return 1;
    }

    // 获取文件状态信息，包括文件大小
    if (fstat(fd, &fileStat) == -1)
    {
        perror("Error getting file status");
        close(fd);
        return 1;
    }

    int file_size = fileStat.st_size;
    // 分配足够大的内存空间来存储文件内容
    *buffer = (char *)malloc(file_size);
    if (*buffer == NULL)
    {
        perror("Error allocating memory");
        close(fd);
        return 1;
    }

    // 直接读取文件内容
    bytesRead = read(fd, *buffer, file_size);
    if (bytesRead != file_size)
    {
        perror("Error reading file");
        free(*buffer);
        close(fd);
        return 1;
    }

    close(fd); // 关闭文件

    return file_size;
}