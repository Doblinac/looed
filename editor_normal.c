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

// normal_mode 模式
void handle_normal_mode() {
    curs_set(1); // 显示光标
    int ch = wgetch(editor_win);

    switch (ch) {
        case 'i': // 切换到 Insert 模式
            set_mode(INSERT_MODE);
            break;
        case 'u': // 撤销操作
            undo();
            break;
        case ':': // 切换到 Command 模式
            set_mode(COMMAND_MODE);
            memset(command_buffer, 0, sizeof(command_buffer)); // 清空命令缓冲区
            break;
        case 'h': // 左移光标
            if (cursor_x > 0) {
                cursor_x--; // 向左移动光标
            }
            break;
        case 'l': // 右移光标
            if (cursor_x < wcslen(text_buffer[cursor_y])) {
                cursor_x++; // 向右移动光标
            }
            break;
        case 'j': // 下移光标
            if (cursor_y < max_lines - 1) {
                cursor_y++;
                // 确保光标不越过当前行的最大长度
                cursor_x = my_min(cursor_x, wcslen(text_buffer[cursor_y]));
            }
            break;
        case 'k': // 上移光标
            if (cursor_y > 0) {
                cursor_y--;
                // 确保光标不越过当前行的最大长度
                cursor_x = my_min(cursor_x, wcslen(text_buffer[cursor_y]));
            }
            break;
        case 'y': // 复制当前行到剪贴板
            copy_to_clipboard(text_buffer[cursor_y]);
            break;
        case 'd': // 剪切当前行到剪贴板
            cut_to_clipboard(cursor_y);
            break;
        case 'p': // 粘贴剪贴板内容
            if (get_clipboard_count() > 0) {
                const wchar_t *clipboard_text = get_clipboard_entry(get_clipboard_count() - 1);
                paste_from_clipboard(clipboard_text);
            }
            break;
    }

    // 更新屏幕显示
    render_screen();
}