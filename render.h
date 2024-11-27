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
int my_max(int a, int b);
int my_min(int a, int b);

// 外部变量声明
extern int height, width;                        // 终端窗口高度和宽度

// 函数声明
void render_screen();        // 初始化并渲染整个屏幕
void render_editor_win();    // 渲染编辑区
void render_status_win();    // 渲染状态栏
void render_cmd_win();       // 渲染命令行
void render_cursor_win();    // 渲染光标位置

#endif // RENDER_H
