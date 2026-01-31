#include <stdio.h>
#include "main.h"
#include "gui.h"

int gui_button(Rectangle rect, const char* text)
{
    Vector2 mouse_point = get_scaled_mouse_position();
    DrawRectangleRec(rect, MAGENTA);
    printf("%f %f\n", mouse_point.x, mouse_point.y);
    DrawText(text, rect.x, rect.y, 20, BLACK);
    return 0;
}
