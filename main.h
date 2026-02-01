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
    GameState* game_state;

    JsonObject* text_config;

    JsonObject* font_config;
    const char** font_names;
    Font* fonts;
    int num_fonts;

    JsonObject* texture_config;
    const char** texture_names;
    Texture2D* textures;
    int num_textures;

    JsonObject* sound_config;
    const char** sound_names;
    Sound* sounds;
    int num_sounds;

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
void play_sound(const char* name);

#endif
