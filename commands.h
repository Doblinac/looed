#ifndef COMMANDS_H
#define COMMANDS_H

#include <ncurses.h>

// 命令缓冲区大小
#define COMMAND_BUFFER_SIZE 1024

// 外部变量声明
extern char command_buffer[COMMAND_BUFFER_SIZE];

// 函数声明
void handle_command_mode();

#endif // COMMANDS_H