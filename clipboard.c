#include "clipboard.h"
#include "editor.h"
#include "editor_normal.h"
#include "editor_insert.h"
#include "utils.h"
#include "scroll.h"
#include "render.h"
#include "clipboard.h"
#include "commands.h"
#include <wchar.h>
#include <string.h>
#include <stdio.h>

static wchar_t clipboard[CLIPBOARD_SIZE][MAX_CLIPBOARD_LENGTH] = {{L'\0'}}; // 存储剪贴板内容
static int clipboard_count = 0; // 剪贴板中已存储的内容数量

// 将文本复制到剪贴板
void copy_to_clipboard(const wchar_t *text) {
    if (clipboard_count < CLIPBOARD_SIZE) {
        wcsncpy(clipboard[clipboard_count], text, MAX_CLIPBOARD_LENGTH - 1);
        clipboard[clipboard_count][MAX_CLIPBOARD_LENGTH - 1] = L'\0'; // 确保以空字符结尾
        clipboard_count++;
    } else {
        // 如果剪贴板已满，移除最早的内容并插入新的内容
        for (int i = 1; i < CLIPBOARD_SIZE; i++) {
            wcscpy(clipboard[i - 1], clipboard[i]);
        }
        wcsncpy(clipboard[CLIPBOARD_SIZE - 1], text, MAX_CLIPBOARD_LENGTH - 1);
        clipboard[CLIPBOARD_SIZE - 1][MAX_CLIPBOARD_LENGTH - 1] = L'\0';
    }
}

// 剪切当前行到剪贴板
void cut_to_clipboard(int line) {
    if (line >= 0 && line < 1024 && text_buffer[line][0] != L'\0') {
        copy_to_clipboard(text_buffer[line]); // 先将内容复制到剪贴板
        // 删除选中的行并调整缓冲区
        for (int i = line; i < max_lines - 1; i++) {
            wcscpy(text_buffer[i], text_buffer[i + 1]);
        }
        text_buffer[max_lines - 1][0] = L'\0'; // 清空最后一行
        max_lines--; // 减少总行数
        cursor_y = (line < max_lines) ? line : max_lines - 1; // 调整光标位置
    }
}

// 从剪贴板粘贴到光标所在位置
void paste_from_clipboard(const wchar_t *clipboard_text) {
    if (clipboard_text == NULL) {
        fprintf(stderr, "Error: Clipboard is empty\n");
        return;
    }

    const wchar_t *ptr = clipboard_text;
    while (*ptr) {
        if (*ptr == L'\n') {
            // 换行符：移动到下一行
            if (cursor_y + 1 < MAX_LINES) {
                cursor_x = 0;
                cursor_y++;
            } else {
                fprintf(stderr, "Error: Maximum line count exceeded while pasting\n");
                break;
            }
        } else {
            // 插入字符
            insert_char(*ptr);
        }
        ptr++;
    }
}


// 获取剪贴板中的条目
const wchar_t *get_clipboard_entry(int index) {
    if (index >= 0 && index < clipboard_count) {
        return clipboard[index];
    }
    return NULL; // 如果索引无效，返回空
}

// 获取剪贴板中存储的条目数量
int get_clipboard_count() {
    return clipboard_count;
}
