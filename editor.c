// editor.c
#include "editor.h"
#include <string.h>  // 用于 strcpy 和 memset

Editor *editor;  // 定义 editor 变量

void draw_line(WINDOW *win, int y, int x, int length, chtype ch, int color_pair) {
    wattron(win, COLOR_PAIR(color_pair));  // 设置颜色对
    for (int i = 0; i < length; i++) {
        mvwaddch(win, y, x + i, ch);
    }
    wattroff(win, COLOR_PAIR(color_pair));
}

void init_editor() {
    editor = (Editor *)malloc(sizeof(Editor));
    initscr();
    cbreak();
    noecho();
    keypad(editor->editor_win, TRUE);
    keypad(editor->command_win, TRUE);
    curs_set(1);

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // 创建窗口
    editor->top_border_win = newwin(1, max_x, 0, 0);  // 上边框窗口
    editor->status_win = newwin(1, max_x, 1, 0);  // 状态窗口
    editor->editor_win = newwin(max_y - 5, max_x, 2, 0);  // 主编辑窗口
    editor->bottom_border_win = newwin(1, max_x, max_y - 3, 0);  // 下边框窗口
    editor->status_win = newwin(1, max_x, max_y - 2, 0);  // 状态窗口
    editor->command_win = newwin(1, max_x, max_y - 1, 0);  // 命令窗口

    // 初始化缓冲区
    editor->buffer = create_rope_from_string("");
    editor->cursor_x = 0;
    editor->cursor_y = 0;
    editor->mode = NORMAL_MODE;
    editor->max_y = max_y - 4;
    editor->max_x = max_x;

    wrefresh(editor->top_border_win);
    wrefresh(editor->bottom_border_win);

    // 刷新其他窗口
    wrefresh(editor->editor_win);
    wrefresh(editor->status_win);
    wrefresh(editor->command_win);
}

void cleanup_editor() {
    free_rope(editor->buffer);
    delwin(editor->editor_win);
    delwin(editor->status_win);
    delwin(editor->command_win);
    delwin(editor->top_border_win);
    delwin(editor->bottom_border_win);
    endwin();
    free(editor);
}

void render_editor() {
    wmove(editor->editor_win, 0, 0);
    wclear(editor->editor_win);
    char *text = to_string(editor->buffer);
    mvwaddstr(editor->editor_win, 0, 0, text);
    wmove(editor->editor_win, editor->cursor_y, editor->cursor_x);
    wrefresh(editor->editor_win);
    free(text);
}

void render_status() {
    wclear(editor->status_win);
    
    // 创建一个足够大的缓冲区来存储状态信息
    char status[100];
    
    // 根据模式选择正确的模式字符串
    const char *mode_str = (editor->mode == NORMAL_MODE) ? "NORMAL" :
                           (editor->mode == INSERT_MODE) ? "INSERT" : "COMMAND";
    
    // 使用 snprintf 构造状态信息
    snprintf(status, sizeof(status), "Mode: %s Line: %d, Column: %d",
             mode_str, editor->cursor_y + 1, editor->cursor_x + 1);
    
    // 将状态信息写入窗口
    wprintw(editor->status_win, "%s", status);
    
    wrefresh(editor->status_win);
}

void render_command() {
	char command[100];
    wmove(editor->command_win, 0, 0);
    
        if (editor->mode == COMMAND_MODE) {
        mvwprintw(editor->command_win, 0, 0, ":%s", command);
        wmove(editor->command_win, 0, strlen(":") + strlen(command)); // 移动光标到命令行输入位置
    }
    wclear(editor->command_win);
    wrefresh(editor->command_win);
}

void render_top_and_bottom() {
	    // 绘制并刷新 top_border_win
    wmove(editor->editor_win, 1, 1);
    wclear(editor->top_border_win);
    wattron(editor->top_border_win, COLOR_PAIR(1));  // 设置颜色对
    for (int i = 0; i < editor->max_x; i++) {
        mvwaddch(editor->top_border_win, 0, i, ACS_HLINE);  // 使用 ACS_HLINE 绘制水平线
    }
    wattroff(editor->top_border_win, COLOR_PAIR(1));
    wrefresh(editor->top_border_win);

    // 绘制并刷新 bottom_border_win
    wmove(editor->editor_win, 0, 0);
    wclear(editor->bottom_border_win);
    wattron(editor->bottom_border_win, COLOR_PAIR(1));  // 设置颜色对
    for (int i = 0; i < editor->max_x; i++) {
        mvwaddch(editor->bottom_border_win, 0, i, ACS_HLINE);  // 使用 ACS_HLINE 绘制水平线
    }
    wattroff(editor->bottom_border_win, COLOR_PAIR(1));
    wrefresh(editor->top_border_win);
    wrefresh(editor->bottom_border_win);
}

void render_screen() {
	render_top_and_bottom();
	render_editor();
	render_status();
	render_command();
}


int get_mode() {
    return editor->mode;
}

void set_mode(Mode mode) {
    editor->mode = mode;
}