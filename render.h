#ifndef RENDER_H
#define RENDER_H

#include <ncurses.h>

// 声明全局变量
extern WINDOW *editor_win;
extern WINDOW *status_win;
extern WINDOW *cmd_win;

// 初始化编辑器
void init_editor();

// 渲染屏幕
void render_screen();
int my_max(int a, int b);
int my_min(int a, int b);

#endif // RENDER_H
