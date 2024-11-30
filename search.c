// search.c
#include "editor.h"
#include <string.h>
#include <stdlib.h>

void handle_search_mode() {
    // 实现查找功能
    // 这里只是一个简单的示例
    char search_term[100];
    wmove(editor->command_win, 0, 0);
    werase(editor->command_win);
    wprintw(editor->command_win, "/: ");
    wgetnstr(editor->command_win, search_term, 98);
    wrefresh(editor->command_win);

    // 在缓冲区中查找搜索项
    int found = search_in_rope(editor->buffer, search_term, &editor->cursor_y, &editor->cursor_x);
    if (found) {
        mvwprintw(editor->command_win, 1, 0, "Found: %s", search_term);
    } else {
        mvwprintw(editor->command_win, 1, 0, "Not found: %s", search_term);
    }
    wrefresh(editor->command_win);

    set_mode(NORMAL_MODE);
    wmove(editor->editor_win, editor->cursor_y, editor->cursor_x);
    wrefresh(editor->editor_win);
}

// 假设 search_in_rope 函数已经定义
int search_in_rope(Rope *rope, const char *search_term, int *y, int *x) {
    // 实现查找功能
    // 这里只是一个简单的示例
    char *buffer = rope_to_string(rope);
    char *pos = strstr(buffer, search_term);
    if (pos != NULL) {
        *y = (pos - buffer) / editor->max_x;
        *x = (pos - buffer) % editor->max_x;
        free(buffer);
        return 1;
    }
    free(buffer);
    return 0;
}