#include "../include/console.h"

#define COMMAND_BUFF_SIZE       1024
#define COMMAND_HISTORY_SIZE    1024
#define DEL_KEY                 127
#define ESC_KEY                 27
#define BACKSPACE_KEY           8 

#define PREVIOUS_CMD            0
#define NEXT_CMD                1

static char cmd_buf[COMMAND_BUFF_SIZE];
static int cmd_buf_idx = 0;

static char cmd_history[COMMAND_HISTORY_SIZE][COMMAND_BUFF_SIZE];
static int cmd_history_len[COMMAND_HISTORY_SIZE];
static int cmd_history_cnt = 0;
static int cmd_history_cur = 0;

static uint32 last_duration;

static int arrow_flag;

void retrieve_command_history(int direction)
{
    if(direction == PREVIOUS_CMD)
    {
        if(cmd_history_cur == 0) return;
        else
        {
            printf("\r");
            for(int i = 0; i < cmd_buf_idx + 2; ++ i) printf(" ");
            printf("\rroot:$ ");

            memset(cmd_buf, '\0', COMMAND_BUFF_SIZE);
            cmd_history_cur --;
            strcpy(cmd_buf, cmd_history[cmd_history_cur]);
            cmd_buf_idx = cmd_history_len[cmd_history_cur];
            printf("%s", cmd_buf);
        }
    }
    else
    {
        if(cmd_history_cur == cmd_history_cnt) return;
        else
        {
            printf("\r");
            for(int i = 0; i < cmd_buf_idx + 2; ++ i) printf(" ");
            printf("\rroot:$ ");

            memset(cmd_buf, '\0', COMMAND_BUFF_SIZE);
            cmd_history_cur ++;
            strcpy(cmd_buf, cmd_history[cmd_history_cur]);
            cmd_buf_idx = cmd_history_len[cmd_history_cur];
            printf("%s", cmd_buf);
        }
    }
}

void add_command_history()
{
    memset(cmd_history[cmd_history_cnt ++], '\0', COMMAND_BUFF_SIZE);
    strcpy(cmd_history[cmd_history_cnt - 1], cmd_buf);
    cmd_history_len[cmd_history_cnt - 1] = cmd_buf_idx;
    cmd_history_cur = cmd_history_cnt;
}

int command_empty()
{
    int cnt = 0;
    for(int i = 0; i < cmd_buf_idx; ++ i)
    {
        if(cmd_buf[i] == ' ') cnt ++;
    }
    if(cnt == cmd_buf_idx) return 1;
    else return 0;
}

void console_init()
{
    cmd_buf_idx = 0;
    last_duration = 0;
    arrow_flag = 0;
    cmd_history_cnt = 0;
    cmd_history_cur = 0;

    for(int i = 0; i < COMMAND_HISTORY_SIZE; ++ i)
    {
        memset(cmd_history[i], '\0', COMMAND_BUFF_SIZE);
        cmd_history_len[i] = 0;
    }

    memset(cmd_buf, '\0', COMMAND_BUFF_SIZE);
    printf(ARMY_ANT_LOGO);
    printf("root:$ ");
}

void console_display(char c)
{
    switch (c)
    {
        case DEL_KEY :
        {
            if(cmd_buf_idx > 0)
            {
                printf("\b \b");
            }
            break;
        }
        case BACKSPACE_KEY :
        {
            if(cmd_buf_idx > 0)
            {
                printf("\b \b");
            }
            break;
        }
        case '\r' :
        {
            if(cmd_buf[0] == 'p' && cmd_buf[1] == 's')
            {
                printf("\n\rPID          NAME\n\r");
                printf(" 0          init");
            }
            if(cmd_buf[0] == '.' && cmd_buf[1] == '/')
            {
                printf("\n\rHello World!");
            }
            if(cmd_buf[0] == 'l' && cmd_buf[1] == 's')
            {
                printf("\n\r3月 15 13:57 hello");
            }
            printf("\n\rroot:$ ");
            break;
        }
        case '[' :
        {
            if(arrow_flag)
            {
                if(last_duration > 10)
                {
                    printf("%c", c);
                }
            }
            else
            {
                printf("%c", c);
            }
            break;
        }
        case 'A' :
        {
            if(arrow_flag == 2)
            {
                if(last_duration <= 10)
                {
                    // printf("-UPARROW-");
                }
                else
                {
                    printf("[%c", c);
                }
            }
            else
            {
                printf("%c", c);
            }
            break;
        }
        case 'B' :
        {
            if(arrow_flag == 2)
            {
                if(last_duration <= 10)
                {
                    // printf("-DOWNARROW-");
                }
                else
                {
                    printf("[%c", c);
                }
            }
            else
            {
                printf("%c", c);
            }
            break;
        }
        case 'C' :
        {
            if(arrow_flag == 2)
            {
                if(last_duration <= 10)
                {
                    printf("-RIGHTARROW-");
                }
                else
                {
                    printf("[%c", c);
                }
            }
            else
            {
                printf("%c", c);
            }
            break;
        }
        case 'D' :
        {
            if(arrow_flag == 2)
            {
                if(last_duration <= 10)
                {
                    printf("-LEFTARROW-");
                }
                else
                {
                    printf("[%c", c);
                }
            }
            else
            {
                printf("%c", c);
            }
            break;
        }
        case ESC_KEY :
        {
            break;
        }
        default:
        {
            printf("%c", c);
            break;
        }
            
    }
}

// Remove special charactors from command, and echo to the terminal.
void console_cmd_process(char c)
{
    switch (c)
    {
        case DEL_KEY :
        {
            arrow_flag = 0;
            if(cmd_buf_idx > 0)
            {
                cmd_buf[-- cmd_buf_idx] = '\0';
            }
            break;
        }
        case BACKSPACE_KEY :
        {
            arrow_flag = 0;
            if(cmd_buf_idx > 0)
            {
                cmd_buf[-- cmd_buf_idx] = '\0';
            }
            break;
        }
        case '\r' :
        {
            arrow_flag = 0;
            if(!command_empty())
            {
                add_command_history();
            }
            
            memset(cmd_buf, '\0', COMMAND_BUFF_SIZE);
            cmd_buf_idx = 0;
            break;
        }
        case '[' :
        {
            if(arrow_flag)
            {
                if(last_duration <= 10)
                {
                    arrow_flag ++;
                }
                else
                {
                    arrow_flag = 0;
                    cmd_buf[cmd_buf_idx ++] = c;
                }
            }
            else
            {
                arrow_flag = 0;
                cmd_buf[cmd_buf_idx ++] = c;
            }
            break;
        }
        case 'A' :
        {
            if(arrow_flag == 2)
            {
                if(last_duration <= 10)
                {
                    arrow_flag = 0;
                    retrieve_command_history(PREVIOUS_CMD);
                }
                else
                {
                    arrow_flag = 0;
                    cmd_buf[cmd_buf_idx ++] = '[';
                    cmd_buf[cmd_buf_idx ++] = c;
                }
            }
            else
            {
                arrow_flag = 0;
                cmd_buf[cmd_buf_idx ++] = c;
            }
            break;
        }
        case 'B' :
        {
            if(arrow_flag == 2)
            {
                if(last_duration <= 10)
                {
                    arrow_flag = 0;
                    retrieve_command_history(NEXT_CMD);
                }
                else
                {
                    arrow_flag = 0;
                    cmd_buf[cmd_buf_idx ++] = '[';
                    cmd_buf[cmd_buf_idx ++] = c;
                }
            }
            else
            {
                arrow_flag = 0;
                cmd_buf[cmd_buf_idx ++] = c;
            }
            break;
        }
        case 'C' :
        {
            if(arrow_flag == 2)
            {
                if(last_duration <= 10)
                {
                    arrow_flag = 0;
                }
                else
                {
                    arrow_flag = 0;
                    cmd_buf[cmd_buf_idx ++] = '[';
                    cmd_buf[cmd_buf_idx ++] = c;
                }
            }
            else
            {
                arrow_flag = 0;
                cmd_buf[cmd_buf_idx ++] = c;
            }
            break;
        }
        case 'D' :
        {
            if(arrow_flag == 2)
            {
                if(last_duration <= 10)
                {
                    arrow_flag = 0;
                }
                else
                {
                    arrow_flag = 0;
                    cmd_buf[cmd_buf_idx ++] = '[';
                    cmd_buf[cmd_buf_idx ++] = c;
                }
            }
            else
            {
                arrow_flag = 0;
                cmd_buf[cmd_buf_idx ++] = c;
            }
            break;
        }
        case ESC_KEY :
        {
            arrow_flag ++;
            break;
        }
        default:
        {
            arrow_flag = 0;
            cmd_buf[cmd_buf_idx ++] = c;
            break;
        }
    }
}

void console_get_char(char c)
{
    timing_duration(&last_duration);
    console_display(c);
    console_cmd_process(c);
    start_timing();
}