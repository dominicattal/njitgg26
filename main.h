#ifndef MAIN_H
#define MAIN_H

#include <raylib.h>
#include "game.h"
#include "json.h"
#include "log.h"

#define DEFAULT_TINT ((Color){ 255, 255, 255, 255 })

typedef struct GlobalContext {
    JsonObject* texture_config;
    GameState* game_state;
    Texture2D* textures;
    const char** texture_names;
    int num_textures;
} GlobalContext;

extern GlobalContext ctx;

Texture2D get_texture_from_config(const char* name);

#endif
