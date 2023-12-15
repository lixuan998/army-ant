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
            k_printf("\r");
            for(int i = 0; i < cmd_buf_idx + 2; ++ i) k_printf(" ");
            k_printf("\r$ ");

            k_memset(cmd_buf, '\0', COMMAND_BUFF_SIZE);
            cmd_history_cur --;
            k_strcpy(cmd_buf, cmd_history[cmd_history_cur]);
            cmd_buf_idx = cmd_history_len[cmd_history_cur];
            k_printf("%s", cmd_buf);
        }
    }
    else
    {
        if(cmd_history_cur == cmd_history_cnt) return;
        else
        {
            k_printf("\r");
            for(int i = 0; i < cmd_buf_idx + 2; ++ i) k_printf(" ");
            k_printf("\r$ ");

            k_memset(cmd_buf, '\0', COMMAND_BUFF_SIZE);
            cmd_history_cur ++;
            k_strcpy(cmd_buf, cmd_history[cmd_history_cur]);
            cmd_buf_idx = cmd_history_len[cmd_history_cur];
            k_printf("%s", cmd_buf);
        }
    }
}

void add_command_history()
{
    k_memset(cmd_history[cmd_history_cnt ++], '\0', COMMAND_BUFF_SIZE);
    k_strcpy(cmd_history[cmd_history_cnt - 1], cmd_buf);
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
        k_memset(cmd_history[i], '\0', COMMAND_BUFF_SIZE);
        cmd_history_len[i] = 0;
    }

    k_memset(cmd_buf, '\0', COMMAND_BUFF_SIZE);
    k_printf(ARMY_ANT_LOGO);
    k_printf("$ ");
}

void console_display(char c)
{
    switch (c)
    {
        case DEL_KEY :
        {
            if(cmd_buf_idx > 0)
            {
                k_printf("\b \b");
            }
            break;
        }
        case BACKSPACE_KEY :
        {
            if(cmd_buf_idx > 0)
            {
                k_printf("\b \b");
            }
            break;
        }
        case '\r' :
        {
            k_printf("\n\rcommad: %s", cmd_buf);
            k_printf("\n\r$ ");
            break;
        }
        case '[' :
        {
            if(arrow_flag)
            {
                if(last_duration > 10)
                {
                    k_printf("%c", c);
                }
            }
            else
            {
                k_printf("%c", c);
            }
            break;
        }
        case 'A' :
        {
            if(arrow_flag == 2)
            {
                if(last_duration <= 10)
                {
                    // k_printf("-UPARROW-");
                }
                else
                {
                    k_printf("[%c", c);
                }
            }
            else
            {
                k_printf("%c", c);
            }
            break;
        }
        case 'B' :
        {
            if(arrow_flag == 2)
            {
                if(last_duration <= 10)
                {
                    // k_printf("-DOWNARROW-");
                }
                else
                {
                    k_printf("[%c", c);
                }
            }
            else
            {
                k_printf("%c", c);
            }
            break;
        }
        case 'C' :
        {
            if(arrow_flag == 2)
            {
                if(last_duration <= 10)
                {
                    k_printf("-RIGHTARROW-");
                }
                else
                {
                    k_printf("[%c", c);
                }
            }
            else
            {
                k_printf("%c", c);
            }
            break;
        }
        case 'D' :
        {
            if(arrow_flag == 2)
            {
                if(last_duration <= 10)
                {
                    k_printf("-LEFTARROW-");
                }
                else
                {
                    k_printf("[%c", c);
                }
            }
            else
            {
                k_printf("%c", c);
            }
            break;
        }
        case ESC_KEY :
        {
            break;
        }
        default:
        {
            k_printf("%c", c);
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
            
            k_memset(cmd_buf, '\0', COMMAND_BUFF_SIZE);
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
    k_stop_timing(&last_duration);
    console_display(c);
    console_cmd_process(c);
    k_start_timing();
}