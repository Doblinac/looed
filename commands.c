#include "commands.h"
#include "editor.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

extern char text_buffer[1024][1024]; // 引用编辑缓冲区
// 引用全局变量
extern char command_buffer[COMMAND_BUFFER_SIZE];
// extern char text_buffer[][MAX_COLS];
extern int cursor_x, cursor_y;
extern WINDOW *cmd_win;

// 命令模式处理
void handle_command_mode() {
    int ch;
    int cmd_len = 0;

    memset(command_buffer, 0, COMMAND_BUFFER_SIZE);

    while ((ch = wgetch(cmd_win)) != '\n') {
        if (ch == 27) { // ESC 返回 Normal 模式
            set_mode(NORMAL_MODE);
            return;
        } else if (ch == KEY_BACKSPACE || ch == 127) {
            if (cmd_len > 0) {
                command_buffer[--cmd_len] = '\0';
                werase(cmd_win);
                mvwprintw(cmd_win, 0, 0, ":%s", command_buffer);
                wrefresh(cmd_win);
            }
        } else if (cmd_len < COMMAND_BUFFER_SIZE - 1) {
            command_buffer[cmd_len++] = ch;
            command_buffer[cmd_len] = '\0';
            werase(cmd_win);
            mvwprintw(cmd_win, 0, 0, ":%s", command_buffer);
            wrefresh(cmd_win);
        }
    }

    // 处理命令
    if (strcmp(command_buffer, "i") == 0) {
        set_mode(INSERT_MODE);
        wmove(editor_win, cursor_y + 1, cursor_x + 1); // 移动光标到文本编辑区
        wrefresh(editor_win); // 刷新编辑区以显示光标
    } else if (strcmp(command_buffer, "q") == 0) {
        cleanup_editor();
        exit(0);
    } else if (strcmp(command_buffer, "w") == 0) {
        // 保存文件逻辑
    } else if (strcmp(command_buffer, "x") == 0) {
        cleanup_editor();
        exit(0);
    }

    set_mode(NORMAL_MODE);
}