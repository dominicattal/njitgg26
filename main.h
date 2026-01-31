#ifndef MAIN_H
#define MAIN_H

#include <raylib.h>
#include "game.h"
#include "json.h"

typedef enum CursorEnum {
    CURSOR_NORMAL,
    CURSOR_INTERACT,
    NUM_CURSORS
} CursorEnum;

typedef struct GlobalContext {
    JsonObject* texture_config;
    JsonObject* text_config;
    JsonObject* font_config;
    GameState* game_state;
    Texture2D* textures;
    const char** font_names;
    Font* fonts;
    int num_fonts;
    const char** texture_names;
    int num_textures;
    bool window_exited;
    struct {
        int x, y;
    } resolution;
    Texture cursor_textures[NUM_CURSORS];
    CursorEnum current_cursor;
    Font font;
} GlobalContext;

extern GlobalContext ctx;

void close_window_safely(void);
Vector2 get_scaled_mouse_position(void);
Rectangle create_rect(float x, float y, float width, float height);
Rectangle create_rect2(float x1, float y1, float x2, float y2);
float lerp(float from, float to, float t, float max_t);
void set_cursor(CursorEnum cursor);

#endif
