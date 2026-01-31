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
    game.in_menu = true;
    game.current_screen = SCREEN_1;
    game.screens = malloc(NUM_SCREENS * sizeof(Screen));

    game.screens[SCREEN_1].background_texture_name = "screen1";
    game.screens[SCREEN_1].render = render_screen_1;

    game.screens[SCREEN_2].background_texture_name = "screen2";
    game.screens[SCREEN_2].render = render_screen_2;

    game.screens[SCREEN_3].background_texture_name = "screen3";
    game.screens[SCREEN_3].render = render_screen_3;
}

static void render_menu_stuff(void)
{
    int window_width = GetScreenWidth();
    //int window_height = GetScreenHeight();
    Texture2D tex;
    Rectangle src, dst;
    //Vector2 origin;
    bool pressed;

    tex = get_texture_from_config("placeholder");

    src = (Rectangle) { 0, 0, tex.width, tex.height };
    dst = (Rectangle) { window_width/2-250, 100, 500, 200 };
    //origin = (Vector2) { window_width/2-100, window_height/2-100 };

    DrawTexturePro(tex, src, dst, ZERO_ZERO, 0.0, DEFAULT_TINT);

    pressed = GuiButton((Rectangle){ window_width/2-50, 350, 100, 20}, "#191#Play");
    if (pressed) {
        game.in_menu = false;
    }
}

static void render_game_stuff(void)
{
    ScreenRenderFuncPtr render_screen = game.screens[game.current_screen].render;
    const char* background_texture_name = game.screens[game.current_screen].background_texture_name;
    Texture2D texture = get_texture_from_config(background_texture_name);
    Rectangle src, dst;

    src = (Rectangle) { 0, 0, texture.width, texture.height };
    dst = (Rectangle) { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };

    DrawTexturePro(texture, src, dst, ZERO_ZERO, 0.0, DEFAULT_TINT);

    if (render_screen == NULL)
        log_write(FATAL, "render fucntion for screen %d is null", game.current_screen);

    render_screen();
}

void game_render(void)
{
    if (game.in_menu)
        render_menu_stuff();
    else
        render_game_stuff();
}

void game_cleanup(void)
{
    free(game.screens);
}
