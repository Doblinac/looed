// editor.h
#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>
#include "rope.h"
#include "normal.h"
#include "command.h"
#include "insert.h"
#include "search.h"

typedef enum {
    NORMAL_MODE,
    INSERT_MODE,
    COMMAND_MODE
} Mode;

typedef struct {
    WINDOW *editor_win;
    WINDOW *status_win;
    WINDOW *line_num_win;
    WINDOW *command_win;
    WINDOW *top_border_win;
    WINDOW *bottom_border_win;
    Rope *buffer;
    char filename[100];
    int cursor_x;
    int cursor_y;
    Mode mode;
    int max_y;
    int max_x;
} Editor;

extern Editor *editor;
extern char command[100];

void init_editor();
void cleanup_editor();
void render_editor();
void render_status();
void render_command();
void render_top_and_bottom();
void render_screen() ;
int get_mode();
void set_mode(Mode mode);

#endif // EDITOR_H