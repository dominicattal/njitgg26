#include <raylib.h>
#include "game.h"
#include "main.h"

// persistent game state among screens

void game_init(void)
{
}

void game_render(void)
{
    Texture2D texture = get_texture_from_config("placeholder");
    DrawTexture(texture, ctx.window_width/2 - texture.width/2, ctx.window_height/2 - texture.height/2, WHITE);
}

void game_cleanup(void)
{
}
