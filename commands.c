#include "commands.h"
#include "clipboard.h"
#include "render.h"
#include "editor.h"
#include "editor_normal.h"
#include "editor_insert.h"
#include "utils.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

// 引用全局变量
extern char command_buffer[COMMAND_BUFFER_SIZE];
extern int cursor_x, cursor_y;
extern WINDOW *cmd_win;

// 命令模式处理
void handle_command_mode() {
    int ch;
    int cmd_len = 0;

    memset(command_buffer, 0, COMMAND_BUFFER_SIZE);

    // 捕获用户输入
    while ((ch = wgetch(cmd_win)) != '\n') {
        if (ch == KEY_BACKSPACE || ch == 127) {  // 处理退格键
            if (cmd_len > 0) {
                command_buffer[--cmd_len] = '\0';
            }
        } else {
            if (cmd_len < COMMAND_BUFFER_SIZE - 1) {
                command_buffer[cmd_len++] = ch;
                command_buffer[cmd_len] = '\0';
            }
        }

        // 显示命令输入
        werase(cmd_win); // 清空命令窗口
        mvwprintw(cmd_win, 0, 0, ":%s", command_buffer);
        wrefresh(cmd_win); // 刷新窗口显示
    }

    // 处理输入命令
    if (strcmp(command_buffer, "i") == 0) {
        set_mode(INSERT_MODE);
    } else if (strcmp(command_buffer, "q") == 0) {
        cleanup_editor();
        exit(0);
    } else if (strcmp(command_buffer, "w") == 0) {
        save_file(editor_filename);
    } else if (strcmp(command_buffer, "wq") == 0) {
        handle_wq_command();
    } else {
        // 未识别的命令
        mvwprintw(cmd_win, 0, 0, "Unknown command: %s", command_buffer);
        wrefresh(cmd_win);
    }

    // 返回 Normal 模式
    set_mode(NORMAL_MODE);
}