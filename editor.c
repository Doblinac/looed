#include "editor.h"
#include "commands.h"
#include <ncurses.h>
#include <string.h>

// 全局变量
Mode current_mode = NORMAL_MODE; // 初始为 Normal 模式
char text_buffer[1024][1024] = {{'\0'}}; // 支持多行文本
char command_buffer[1024] = {'\0'};      // 命令输入缓冲
int cursor_x = 0, cursor_y = 0, max_lines = 0;
WINDOW *editor_win, *status_win, *cmd_win;

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
    // 编辑区渲染
    werase(editor_win);
    box(editor_win, 0, 0);
    for (int i = 0; i <= max_lines; i++) {
        mvwprintw(editor_win, i + 1, 1, "%s", text_buffer[i]);
    }
    // 移动光标到文本编辑区
    wmove(editor_win, cursor_y + 1, cursor_x + 1);

    // 刷新编辑区以显示光标
    wrefresh(editor_win);

    // 状态栏渲染
    werase(status_win);
    switch (current_mode) {
        case NORMAL_MODE:
            mvwprintw(status_win, 0, 0, "[NORMAL MODE] Press i to insert, : for command");
            break;
        case INSERT_MODE:
            mvwprintw(status_win, 0, 0, "[INSERT MODE] Press ESC to return to normal mode");
            break;
        case COMMAND_MODE:
            mvwprintw(status_win, 0, 0, "[COMMAND MODE] Type your command");
            break;
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
        wmove(editor_win, cursor_y + 1, cursor_x + 1); // 光标在文本编辑区
        curs_set(1); // 显示光标
    } else if (current_mode == COMMAND_MODE) {
        wmove(cmd_win, 0, strlen(":") + strlen(command_buffer)); // 光标在命令行
        curs_set(1); // 显示光标
    }
}

// 获取当前模式
Mode get_mode() {
    return current_mode;
}

// 设置模式
void set_mode(Mode mode) {
    current_mode = mode;
}

// Normal 模式处理
void handle_normal_mode() {
	curs_set(0);            // 显示光标
    int ch = wgetch(editor_win);
    switch (ch) {
        case 'i': // 切换到 Insert 模式
           set_mode(INSERT_MODE);
           wmove(editor_win, cursor_y + 1, cursor_x + 1); // 移动光标到文本编辑区
           curs_set(1);            // 显示光标
           render_screen();
            break;
        case ':': // 切换到 Command 模式
            set_mode(COMMAND_MODE);
            memset(command_buffer, 0, sizeof(command_buffer)); // 清空命令缓冲区
            break;
    }
}

// 插入模式处理
void handle_insert_mode() {
    int ch = wgetch(editor_win);
    switch (ch) {
        case 27: // ESC 切换到 Normal 模式
            set_mode(NORMAL_MODE);
            break;
        case KEY_BACKSPACE:
        case 127:
            if (cursor_x > 0) {
                cursor_x--;
                text_buffer[cursor_y][cursor_x] = '\0';
            } else if (cursor_y > 0) {
                cursor_x = strlen(text_buffer[cursor_y - 1]);
                strcat(text_buffer[cursor_y - 1], text_buffer[cursor_y]);
                for (int i = cursor_y; i < max_lines; i++) {
                    strcpy(text_buffer[i], text_buffer[i + 1]);
                }
                max_lines--;
                cursor_y--;
            }
            break;
        case '\n': // 回车换行
            for (int i = max_lines; i > cursor_y; i--) {
                strcpy(text_buffer[i + 1], text_buffer[i]);
            }
            max_lines++;
            strcpy(text_buffer[cursor_y + 1], text_buffer[cursor_y] + cursor_x);

            text_buffer[cursor_y][cursor_x] = '\0';
            cursor_x = 0;
            cursor_y++;
            break;
        default:
            text_buffer[cursor_y][cursor_x] = ch;
            cursor_x++;
            break;
    }
    render_screen(); // 更新屏幕显示，包括光标位置
}

char history_buffer[HISTORY_SIZE][1024][1024] = {{{'\0'}}}; // 撤销历史记录栈
int history_index = 0; // 当前历史记录的索引

// 保存当前状态到历史记录
void save_to_history() {
    if (history_index < HISTORY_SIZE - 1) {
        memcpy(history_buffer[history_index + 1], text_buffer, sizeof(text_buffer));
        history_index++;
    }
}

// 撤销操作
void undo() {
    if (history_index > 0) {
        memcpy(text_buffer, history_buffer[history_index - 1], sizeof(text_buffer));
        history_index--;
        render_screen(); // 重新渲染屏幕
    }
}



// 清理编辑器
void cleanup_editor() {
    delwin(editor_win);
    delwin(status_win);
    delwin(cmd_win);
    endwin();
}