// main.c
#include "editor.h"

int main(int argc, char *argv[]) {
    init_editor();
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);  // 初始化颜色对

    if (argc > 1) {
        open_file(argv[1]);
    }

    while (1) {
        render_screen();

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

    return 0;
}