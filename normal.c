// normal.c
#include "editor.h"

void handle_normal_mode() {
    curs_set(1);
    int nu = 1;
    render_editor();
    char command[100];
    
    nu = wgetch(editor->editor_win); //     
    switch (nu) {
        case 'k':
            if (editor->cursor_y > 0) {
                editor->cursor_y--;
            }
            break;
        case 'j':
            if (editor->cursor_y < editor->max_y - 1) {
                editor->cursor_y++;
            }
            break;
        case 'h':
            if (editor->cursor_x > 0) {
                editor->cursor_x--;
            }
            break;
        case 'l':
            if (editor->cursor_x < editor->max_x - 1) {
                editor->cursor_x++;
            }
            break;
        case 'i':
            set_mode(INSERT_MODE);
            break;
        case ':':
            set_mode(COMMAND_MODE);
            wmove(editor->command_win, 0, 0);
            mvwprintw(editor->command_win, 0, 0, "Unknown command: %s", command);
            wrefresh(editor->command_win);
            break;
        case 'q':
            cleanup_editor();
            endwin();
            exit(0);
        default:
            break;
            
    }
    wmove(editor->editor_win, editor->cursor_y, editor->cursor_x);
    wrefresh(editor->editor_win);
}