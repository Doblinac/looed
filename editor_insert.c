#include "editor.h"
#include "editor_normal.h"
#include "editor_insert.h"
#include "utils.h"
#include "render.h"
#include "scroll.h"
#include "clipboard.h"
#include "commands.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>
#define CTRL(x) ((x) & 0x1F)
#define CTRL_J 128 // 新的常量值

// 插入模式保持函数
void handle_insert_mode() {
    int ch = wgetch(editor_win);

    switch (ch) {
        case 27: // ESC 切换到 Normal 模式
            set_mode(NORMAL_MODE);
            break;

        case KEY_BACKSPACE: // 兼容系统的退格键
        case 127:
            if (cursor_x > 0) {
                cursor_x--;
                text_buffer[cursor_y][cursor_x] = L'\0';
            } else if (cursor_y > 0) {
                cursor_x = wcslen(text_buffer[cursor_y - 1]);
                wcscat(text_buffer[cursor_y - 1], text_buffer[cursor_y]);
                for (int i = cursor_y; i < max_lines - 1; i++) {
                    wcscpy(text_buffer[i], text_buffer[i + 1]);
                }
                max_lines--;
                cursor_y--;
            }
            break;

        case KEY_LEFT: // 左箭头
            if (cursor_x > 0) {
                cursor_x--;
            }
            break;

        case KEY_RIGHT: // 右箭头
            if (cursor_x < wcslen(text_buffer[cursor_y])) {
                cursor_x++;
            }
            break;

        case KEY_DOWN: // 下箭头
            if (cursor_y < max_lines - 1) {
                cursor_y++;
                cursor_x = my_min(cursor_x, wcslen(text_buffer[cursor_y])); // 确保光标在行内
            }
            break;

        case KEY_UP: // 上箭头
            if (cursor_y > 0) {
                cursor_y--;
                cursor_x = my_min(cursor_x, wcslen(text_buffer[cursor_y])); // 确保光标在行内
            }
            break;

        case '\n': // 回车换行
            for (int i = max_lines; i > cursor_y; i--) {
                wcscpy(text_buffer[i + 1], text_buffer[i]);
            }
            max_lines++;
            wcscpy(text_buffer[cursor_y + 1], &text_buffer[cursor_y][cursor_x]);
            text_buffer[cursor_y][cursor_x] = L'\0';
            cursor_x = 0;
            cursor_y++;
            break;

        default: // 普通字符输入
            if (iswprint(ch)) { // 确保是可打印字符
                insert_char((wchar_t)ch);
            }
            break;
    }

    render_screen(); // 每次输入后刷新屏幕
}
