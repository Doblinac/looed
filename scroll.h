#ifndef SCROLL_H
#define SCROLL_H

#define SCREEN_WIDTH 52
#define SCREEN_HEIGHT 24

#include "editor.h"
#include "utils.h"
#include "render.h"
#include "clipboard.h"

void scroll_down() ;
void scroll_up() ;
void scroll_left() ;
void scroll_right() ;
void insert_char(wchar_t ch);

#endif // SCROLL_H