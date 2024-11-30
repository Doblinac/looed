#include "editor.h"
#include "search.h"
#include "rope.h"
#include <string.h>
#include <stdlib.h>

void handle_command_mode() {
    char command[100];
    int ch;
    size_t cmd_index = 0;  // 将 cmd_index 的类型更改为 size_t

    // 移动光标到命令窗口的起始位置
    wmove(editor->command_win, 0, 0);

    // 清空命令窗口
    werase(editor->command_win);

    // 显示命令提示符
    wprintw(editor->command_win, ":");

    // 刷新命令窗口
    wrefresh(editor->command_win);

    // 读取用户输入的命令
    while ((ch = wgetch(editor->command_win)) != '\n' && cmd_index < sizeof(command) - 1) {
        if (ch == KEY_BACKSPACE || ch == 127) { // 处理退格键
            if (cmd_index > 0) {
                command[--cmd_index] = '\0';
                mvwprintw(editor->command_win, 0, 0, ":%s", command);
                wrefresh(editor->command_win);
            }
        } else {
            command[cmd_index++] = ch;
            mvwprintw(editor->command_win, 0, 0, ":%s", command);
            wrefresh(editor->command_win);
        }
    }
    command[cmd_index] = '\0'; // 确保字符串以空字符结尾

    // 处理命令
          if (strcmp(command, "q") == 0) {
               cleanup_editor();
               endwin();
                exit(0);
              } else if (strcmp(command, "w") == 0) {
                 save_file(editor->filename);
               } else if (strcmp(command, "wq") == 0) {
                 save_file(editor->filename);
                   cleanup_editor();
                   endwin();
                   exit(0);
                } else if (strcmp(command, "s") == 0) {
                  handle_search_mode();
                 } else {
                   mvwprintw(editor->command_win, 1, 0, "Unknown command: %s", command);
                    wrefresh(editor->command_win);
                     set_mode(NORMAL_MODE);
                    wmove(editor->editor_win, editor->cursor_y, editor->cursor_x);
                    wrefresh(editor->editor_win);
                  }
}
