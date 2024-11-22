#include <ncurses.h>
#include "editor.h"
#include "commands.h"

int main() {
    initscr();              // 初始化屏幕
    cbreak();               // 禁用行缓冲
    noecho();               // 禁用自动回显
    keypad(stdscr, TRUE);   // 启用功能键
    curs_set(0);            // 显示光标

    // 初始化编辑器
    init_editor();

    // 主循环
    while (1) {
        render_screen(); // 渲染屏幕
        switch (get_mode()) {
            case NORMAL_MODE:
                handle_normal_mode();
                break;
            case INSERT_MODE:
                handle_insert_mode();
                break;
            case COMMAND_MODE:
                handle_command_mode();
                break;
        }
    }

    // 清理并退出
    cleanup_editor();
    return 0;
}