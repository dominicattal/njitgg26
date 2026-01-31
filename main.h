#ifndef MAIN_H
#define MAIN_H

#include <raylib.h>
#include "game.h"
#include "json.h"
#include "log.h"

typedef struct GlobalContext {
    JsonObject* texture_config;
    GameState* game_state;
    Texture2D* textures;
    const char** texture_names;
    int num_textures;
    int window_width;
    int window_height;
} GlobalContext;

extern GlobalContext ctx;

Texture2D get_texture_from_config(const char* name);

#endif
