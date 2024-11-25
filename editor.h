#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>
#include <wchar.h>
#include <stdbool.h>

#define MAX_LINE_LENGTH 1024
#define MAX_LINES 1024
#define HISTORY_SIZE 100

extern int scroll_x;
extern int scroll_y;
extern int cursor_x;
extern int cursor_y;
extern int max_lines;

typedef enum {
    NORMAL_MODE,
    INSERT_MODE,
    COMMAND_MODE
} Mode;

extern Mode current_mode;
extern char editor_filename[256];
extern wchar_t text_buffer[MAX_LINES][MAX_LINE_LENGTH];
extern char command_buffer[1024];
extern int cursor_x, cursor_y, max_lines;

void set_mode(Mode mode);
Mode get_mode();
void render_screen();
void open_file(const char *filename);
void save_file(const char *filename);
void cleanup_editor();

#endif // EDITOR_H
