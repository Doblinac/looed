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

wchar_t text_buffer[MAX_LINES][MAX_LINE_LENGTH]; // 假设您的文本缓冲区是这样的

int height, width;
int cursor_y, cursor_x;
int curs_y, curs_x;

// 初始化编辑器
void init_editor() {
    getmaxyx(stdscr, height, width);

    // 窗口分配
    editor_win = newwin(height - 3, width, 1, 0);
    status_win = newwin(1, width, height - 2, 0);
    cmd_win = newwin(1, width, height - 1, 0);

     render_screen() ;
}
// 渲染屏幕
void render_screen() {

    // 设置固定窗口框架
    box(editor_win, 0, 0);
       
    render_editor_win() ;
    render_status_win() ;
    render_cmd_win() ;
    render_cursor_win();
    curs_y = getcury(stdscr);
    curs_x = getcurx(stdscr);

}

// 渲染编辑区
void render_editor_win() {
    werase(editor_win);
    box(editor_win, 0, 0);

    int visible_lines = height - 4; // 可见行数

    for (int y = 0;  y < visible_lines && y  < MAX_LINES; y++) {
        for (int x = 0; x < SCREEN_WIDTH && x < MAX_LINE_LENGTH; x++) {
            wchar_t ch = text_buffer[y][x];
            mvwaddch(editor_win, y + 1, x + 1, (ch >= 32 && ch <= 126) ? ch : ' ');
        }
    }

    wrefresh(editor_win);
}

// 渲染状态栏
void render_status_win() {
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
}

// 渲染命令行
void render_cmd_win() {
    werase(cmd_win);

    if (current_mode == COMMAND_MODE) {
        mvwprintw(cmd_win, 0, 0, ":%s", command_buffer);
        wmove(cmd_win, 0, strlen(":") + strlen(command_buffer)); // 移动光标到命令行输入位置
    }

    wrefresh(cmd_win);
}

// 初始化光标
void render_cursor_win() {
    int cursor_win_y = cursor_y - scroll_y + 1;
    int cursor_win_x = cursor_x - scroll_x + 1;

    if (current_mode == NORMAL_MODE || current_mode == INSERT_MODE) {
        // 确保光标在编辑区
        if (cursor_win_y >= 0 && cursor_win_y < height - 4 &&
            cursor_win_x >= 0 && cursor_win_x < width) {
            wmove(editor_win, cursor_win_y, cursor_win_x );
        }
        curs_set(1); // 显示光标
    } else if (current_mode == COMMAND_MODE) {
        // 确保光标在命令行
        wmove(cmd_win, 0, strlen(":") + strlen(command_buffer));
        curs_set(1); // 显示光标
    }
}