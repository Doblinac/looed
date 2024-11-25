#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <wchar.h>

// 剪贴板大小和每条内容的最大长度
#define CLIPBOARD_SIZE 10
#define MAX_CLIPBOARD_LENGTH 1024

// 剪贴板接口
void copy_to_clipboard(const wchar_t *text);
void cut_to_clipboard(int line);
void paste_from_clipboard(const wchar_t *clipboard_text);
const wchar_t *get_clipboard_entry(int index);
int get_clipboard_count();

#endif // CLIPBOARD_H
