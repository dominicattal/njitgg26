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
    Rectangle src, dst;
    //DrawTexture(texture, ctx.window_width/2 - texture.width/2, ctx.window_height/2 - texture.height/2, WHITE);
    src.x = 0;
    src.y = 0;
    src.width = texture.width;
    src.height = texture.height;
    dst.x = 0;
    dst.y = 0;
    dst.width = ctx.window_width;
    dst.height = ctx.window_height;

    Vector2 origin = (Vector2) { 0, 0 };
    Color tint = (Color) { 255, 255, 255, 255 };
    DrawTexturePro(texture, src, dst, origin, 0.0, tint);
}

void game_cleanup(void)
{
}
