// insert.c
#include "editor.h"
#include <wchar.h>

void handle_insert_mode() {
    curs_set(1);
    render_editor();
    int ch = wgetch(editor->editor_win);
    char ch_str[2];
    switch (ch) {
        case KEY_BACKSPACE:
            if (editor->cursor_x > 0) {
                int start = editor->cursor_y * editor->max_x + editor->cursor_x - 2;
                int end = start + 1;
                editor->buffer = delete_rope(editor->buffer, start, end);
                editor->cursor_x--;
               wrefresh(editor->editor_win);
            }
            break;
        case KEY_ENTER:
            editor->buffer = insert_rope(editor->buffer, editor->cursor_y * editor->max_x + editor->cursor_x, "\n");
            editor->cursor_x = 0;
            editor->cursor_y++;
            wrefresh(editor->editor_win);
            break;
        case 27:  // ESC
            set_mode(NORMAL_MODE);
            wrefresh(editor->editor_win);            
            break;
        default:
            sprintf(ch_str, "%c", ch);
            if (ch_str[0] == '\n' ) {
            	 editor->cursor_y++;
                 editor->cursor_x = -1;
                editor->buffer = insert_rope(editor->buffer, editor->cursor_y * editor->max_x + editor->cursor_x, ch_str);
                editor->cursor_x++;                 
            }else if (iswprint(*ch_str)) { // 确保是可打印字符
                editor->buffer = insert_rope(editor->buffer, editor->cursor_y * editor->max_x + editor->cursor_x, ch_str);
                editor->cursor_x++;
                wrefresh(editor->editor_win);
            }
            break;
    }
    wrefresh(editor->editor_win);
}