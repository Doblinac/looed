#include "editor.h"
#include "editor_normal.h"
#include "editor_insert.h"
#include "utils.h"
#include "scroll.h"
#include "render.h"
#include "clipboard.h"
#include "commands.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>

WINDOW *editor_win, *status_win, *cmd_win;

// 定义 max 和 min 函数
int my_max(int a, int b) {
    return (a > b) ? a : b;
}

int my_min(int a, int b) {
    return (a < b) ? a : b;
}

// 全局变量设置
WINDOW *editor_win, *status_win, *cmd_win;

// 光标相对窗口的偏移量
int offset_y = 0;

wchar_t text_buffer[MAX_LINES][MAX_LINE_LENGTH]; // 假设您的文本缓冲区是这样的


// 初始化编辑器
void init_editor() {
    int height, width;
    getmaxyx(stdscr, height, width);

    // 窗口分配
    editor_win = newwin(height - 3, width, 1, 0);
    status_win = newwin(1, width, height - 2, 0);
    cmd_win = newwin(1, width, height - 1, 0);
}

// 渲染屏幕
void render_screen() {
    int height, width;
    getmaxyx(stdscr, height, width); // 获取当前终端尺寸

    // 编辑区渲染
    werase(editor_win);
    box(editor_win, 0, 0);

    // 确保光标始终在窗口中间偏下的可见区域
    int visible_lines = height - 4; // 可见行数
    for (int y = 0; y < SCREEN_HEIGHT && y + scroll_y < MAX_LINES; y++) {
        for (int x = 0; x < SCREEN_WIDTH && x + scroll_x < MAX_LINE_LENGTH; x++) {
         wchar_t ch = text_buffer[y + scroll_y][x + scroll_x];
         if (ch >= 32 && ch <= 126) { // 打印字符范围
                  mvwaddch(editor_win, y + 1, x + 1, ch);
          } else {
           mvwaddch(editor_win, y + 1, x + 1, ' '); // 用空格代替非法字符
             }
        }
    }

    int cursor_win_y = cursor_y - scroll_y+1;
    int cursor_win_x = cursor_x - scroll_x+1;
    if (cursor_win_y >= 0 && cursor_win_y < SCREEN_HEIGHT &&
        cursor_win_x >= 0 && cursor_win_x < SCREEN_WIDTH) {
        wmove(editor_win, cursor_win_y+1, cursor_win_x);
    }

    // 刷新编辑区以显示光标
    wmove(editor_win, cursor_win_y + 1, cursor_x + 1); // 将光标移动到窗口中的正确位置
    wrefresh(editor_win);

    // 状态栏渲染
// 状态栏显示剪贴板信息
      werase(status_win);
      if (current_mode == NORMAL_MODE) {
          if (get_clipboard_count() > 0) {
              const wchar_t *clipboard_text = get_clipboard_entry(get_clipboard_count() - 1);
             mvwprintw(status_win, 0, 0, "[NORMAL MODE] Clipboard: %ls", clipboard_text);
                   } else {
            mvwprintw(status_win, 0, 0, "[NORMAL MODE] Clipboard is empty");
                   }
       } else if (current_mode == INSERT_MODE) {
            mvwprintw(status_win, 0, 0, "[INSERT MODE] Press ESC to return to normal mode");
       } else if (current_mode == COMMAND_MODE) {
            mvwprintw(status_win, 0, 0, "[COMMAND MODE] Type your command");
    }
    wrefresh(status_win);

    // 命令行
    werase(cmd_win);
    if (current_mode == COMMAND_MODE) {
        mvwprintw(cmd_win, 0, 0, ":%s", command_buffer);
        wmove(cmd_win, 0, strlen(":") + strlen(command_buffer)); // 移动光标到命令模式下的位置
    }
    wrefresh(cmd_win);

    // 确保光标在正确位置
    if (current_mode == NORMAL_MODE || current_mode == INSERT_MODE) {
        wmove(editor_win, cursor_win_y, cursor_x + 1); // 光标在文本编辑区
        curs_set(1); // 显示光标
    } else if (current_mode == COMMAND_MODE) {
        wmove(cmd_win, 0, strlen(":") + strlen(command_buffer)); // 光标在命令行
        curs_set(1); // 显示光标
    }
}
