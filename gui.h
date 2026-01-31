#ifndef GUI_H
#define GUI_H

#include <raylib.h>

#define GUI_NONE    0
#define GUI_HOVERED 1
#define GUI_PRESSED 2

int gui_button(Rectangle rect, const char* text);

#endif
