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
} GlobalContext;

extern GlobalContext ctx;

Texture2D get_texture_from_config(const char* name);
void close_window_safely(void);

#endif
