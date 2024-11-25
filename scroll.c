#include "editor.h"
#include "editor_normal.h"
#include "editor_insert.h"
#include "utils.h"
#include "render.h"
#include "clipboard.h"
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <string.h>
#include <ncurses.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24

int scroll_x = 0;
int scroll_y = 0;

void scroll_up() {
    if (scroll_y > 0) {
        scroll_y--;
    }
}

void scroll_down() {
    if (scroll_y < max_lines - SCREEN_HEIGHT) {
        scroll_y++;
    }
}

void scroll_left() {
    if (scroll_x > 0) {
        scroll_x--;
    }
}

void scroll_right() {
    if (scroll_x < MAX_LINE_LENGTH - SCREEN_WIDTH) {
        scroll_x++;
    }
}

void insert_char(wchar_t ch) {
    // 当前行未满，可以直接插入字符
    if (cursor_x < MAX_LINE_LENGTH - 1) {
        size_t line_length = wcslen(text_buffer[cursor_y]);

        if (line_length < MAX_LINE_LENGTH - 1) {
            // 将光标后的字符右移，为新字符腾出空间
            wmemmove(&text_buffer[cursor_y][cursor_x + 1], &text_buffer[cursor_y][cursor_x], line_length - cursor_x);

            // 插入新字符
            text_buffer[cursor_y][cursor_x] = ch;
            cursor_x++;

            // 如果光标超出屏幕宽度，滚动屏幕
            if (cursor_x - scroll_x >= SCREEN_WIDTH) {
                scroll_x++;
            }
        }
    } 
    // 当前行已满，需要将溢出的部分移到下一行
    else if (cursor_y + 1 < MAX_LINES) {
        size_t line_length = wcslen(text_buffer[cursor_y]);

        // 如果是新行，则增加总行数
        if (cursor_y + 1 == max_lines) {
            max_lines++;
        }

        // 将光标位置后的字符移到下一行的开头
        wmemmove(text_buffer[cursor_y + 1], &text_buffer[cursor_y][cursor_x], line_length - cursor_x + 1);

        // 插入新字符
        text_buffer[cursor_y][cursor_x] = ch;

        // 截断当前行
        text_buffer[cursor_y][cursor_x + 1] = L'\0';

        // 光标移动到下一行的行首
        cursor_x = 0;
        cursor_y++;

        // 如果光标超出屏幕高度，滚动屏幕
        if (cursor_y - scroll_y >= SCREEN_HEIGHT) {
            scroll_y++;
        }
    } 
    // 行数达到限制，需要进行滚屏操作
    else if (max_lines == MAX_LINES) {
        // 滚动所有行向上
        scroll_up();

        // 在最后一行插入新字符
        text_buffer[max_lines - 1][0] = ch;
        text_buffer[max_lines - 1][1] = L'\0';

        // 光标移动到最后一行的下一个位置
        cursor_x = 1;
        cursor_y = max_lines - 1;
    } 
    // 行数达到限制，无法插入更多字符
    else {
        fprintf(stderr, "Error: Maximum line count exceeded\n");
    }

    render_screen();
}
