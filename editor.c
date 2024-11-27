#include "editor.h"
#include "editor_normal.h"
#include "editor_insert.h"
#include "utils.h"
#include "scroll.h"
#include "render.h"
#include "clipboard.h"
#include "commands.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>
#define CTRL(x) ((x) & 0x1F)
#define CTRL_J 128 // 新的常量值

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#include <stdbool.h>

// 全局变量
Mode current_mode = NORMAL_MODE; // 初始为 Normal 模式
// 全局变量
char editor_filename[256] = {'\0'}; // 编辑器文件名
char command_buffer[1024] = {'\0'};      // 命令输入缓冲
int max_lines = 0;

// 获取当前模式
Mode get_mode() {
    return current_mode;
}

// 设置模式
void set_mode(Mode mode) {
    current_mode = mode;
}
