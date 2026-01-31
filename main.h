#ifndef MAIN_H
#define MAIN_H

#include <raylib.h>
#include "game.h"
#include "json.h"

typedef struct GlobalContext {
    JsonObject* texture_config;
    GameState* game_state;
    Texture2D* textures;
    const char** texture_names;
    int num_textures;
    bool window_exited;
    struct {
        int x, y;
    } resolution;
} GlobalContext;

extern GlobalContext ctx;

Texture2D get_texture_from_config(const char* name);
void close_window_safely(void);
Vector2 get_scaled_mouse_position(void);
Rectangle create_rect(float x, float y, float width, float height);
float lerp(float from, float to, float t, float max_t);

#endif
