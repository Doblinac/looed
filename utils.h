#ifndef UTILS_H
#define UTILS_H

#include "editor.h"

void save_to_history();
void undo();
void insert_char(wchar_t ch);
void open_file(const char *filename);
void save_file(const char *filename);
void handle_wq_command();
void cleanup_editor();

#endif // UTILS_H
