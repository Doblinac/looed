// editor.h
#ifndef EDITOR_H
#define EDITOR_H

#include <ncurses.h>

// 添加撤销历史记录的最大长度
#define HISTORY_SIZE 100
// 添加撤销历史记录栈
extern char history_buffer[HISTORY_SIZE][1024][1024];
extern int history_index;


extern WINDOW *editor_win;
extern WINDOW *status_win;
extern WINDOW *cmd_win;

typedef enum { NORMAL_MODE, INSERT_MODE, COMMAND_MODE } Mode;

extern WINDOW *cmd_win; // 声明 cmd_win 为全局变量

// 其他函数声明
void init_editor();
void render_screen();
Mode get_mode();
void set_mode(Mode mode);
void handle_normal_mode();
void handle_insert_mode();
void cleanup_editor();

#endif // EDITOR_H
