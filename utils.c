#include "editor.h"
#include "editor_normal.h"
#include "editor_insert.h"
#include "utils.h"
#include "render.h"
#include "clipboard.h"
#include "commands.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>

// 全局变量
char history_buffer[HISTORY_SIZE][1024][1024] = {{{'\0'}}}; // 撤销历史记录栈
int history_index = 0; // 当前历史记录的索引

// 保存当前状态到历史记录
void save_to_history() {
    if (history_index < HISTORY_SIZE - 1) {
        memcpy(history_buffer[history_index + 1], text_buffer, sizeof(text_buffer));
        history_index++;
    } else {
        // 可选：处理历史记录已满的情况
    }
}

// 撤销操作
void undo() {
    if (history_index > 0) {
        memcpy(text_buffer, history_buffer[history_index - 1], sizeof(text_buffer));
        history_index--;
        render_screen(); // 重新渲染屏幕
    }
}

// 打开文件
void open_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // 清空缓冲区
    memset(text_buffer, 0, sizeof(text_buffer));

    // 逐行读取文件内容
    int i = 0;
    while (fgetws(text_buffer[i], MAX_LINE_LENGTH, file) != NULL) {
        // 移除行尾的换行符
        text_buffer[i][wcslen(text_buffer[i]) - 1] = '\0';

        i++;
        if (i >= MAX_LINES) {
            fprintf(stderr, "Error: File exceeds maximum line count (%d)\n", MAX_LINES);
            break;
        }
    }

    max_lines = i; // 实际行数
    fclose(file);

    // 保存文件名
    strncpy(editor_filename, filename, sizeof(editor_filename) - 1);
    editor_filename[sizeof(editor_filename) - 1] = '\0';

    // 将光标移动到文本末尾
    cursor_y = max_lines - 1;
    cursor_x = wcslen(text_buffer[cursor_y]);

    // 重新渲染屏幕
    render_screen();
}

// 保存文件
void save_file(const char *filename) {
         if (filename == NULL || strcspn(filename, "\n") == 0) {
        mvprintw(LINES - 1, 0, "Error: No filename provided");
        refresh();
        return;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        mvprintw(LINES - 1, 0, "Error: Could not open file %s for writing", filename);
        refresh();
        return;
    }

    // 写入编辑器内容
    for (int i = 0; i < max_lines; i++) {
        fputws(text_buffer[i], file);
        fputws(L"\n", file);

    fclose(file);

    // 提示保存成功
    mvprintw(LINES - 1, 0, "File %s saved successfully", filename);
    refresh();
    }
}

// 处理 wq 命令
void handle_wq_command() {
    save_file(editor_filename);
    cleanup_editor();
    exit(0);
}

// 清理编辑器
void cleanup_editor() {
    delwin(editor_win); // 删除编辑器窗口
    delwin(status_win); // 删除状态窗口
    delwin(cmd_win); // 删除命令窗口
    endwin(); // 结束 ncurses 模式，恢复终端设置
    exit(0); // 安全退出程序
}
