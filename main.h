#ifndef MAIN_H
#define MAIN_H

#include <raylib.h>
#include "json.h"
#include "log.h"

typedef struct GlobalContext {
    JsonObject* texture_config;
} GlobalContext;

Texture2D get_texture_from_config(const char* name);

#endif
