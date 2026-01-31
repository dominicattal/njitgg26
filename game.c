#include <stdlib.h>
#include "game.h"
#include "main.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// persistent game state among screens

GameState game;

static void render_screen_1(void)
{
    bool pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go Next");
    if (pressed) {
        game.current_screen = SCREEN_2;
    }
}

static void render_screen_2(void)
{
    bool pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go Previous");
    if (pressed) {
        game.current_screen = SCREEN_1;
    }
    pressed = GuiButton((Rectangle){ 150, 24, 120, 30 }, "#191#Go Next");
    if (pressed) {
        game.current_screen = SCREEN_3;
    }
}

static void render_screen_3(void)
{
    bool pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go Previous");
    if (pressed) {
        game.current_screen = SCREEN_2;
    }
}

void game_init(void)
{
    game.current_screen = SCREEN_1;
    game.screens = malloc(NUM_SCREENS * sizeof(Screen));

    game.screens[SCREEN_1].background_texture_name = "screen1";
    game.screens[SCREEN_1].render = render_screen_1;

    game.screens[SCREEN_2].background_texture_name = "screen2";
    game.screens[SCREEN_2].render = render_screen_2;

    game.screens[SCREEN_3].background_texture_name = "screen3";
    game.screens[SCREEN_3].render = render_screen_3;
}

void game_render(void)
{
    ScreenRenderFuncPtr render = game.screens[game.current_screen].render;
    const char* background_texture_name = game.screens[game.current_screen].background_texture_name;
    Texture2D texture = get_texture_from_config(background_texture_name);
    Rectangle src, dst;
    src.x = 0;
    src.y = 0;
    src.width = texture.width;
    src.height = texture.height;
    dst.x = 0;
    dst.y = 0;
    dst.width = (float)GetScreenWidth();
    dst.height = (float)GetScreenHeight();

    Vector2 origin = (Vector2) { 0, 0 };
    DrawTexturePro(texture, src, dst, origin, 0.0, DEFAULT_TINT);

    if (render == NULL)
        log_write(FATAL, "render fucntion for screen %d is null", game.current_screen);

    render();

    //    showMessageBox = true;
    //if (showMessageBox) 
    //{
    //    int result = GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
    //        "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

    //    if (result >= 0) showMessageBox = false;
    //}
}

void game_cleanup(void)
{
    free(game.screens);
}
