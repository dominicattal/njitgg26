#include <stdlib.h>
#include "game.h"
#include "main.h"
#include "gui.h"
#include "raygui.h"

GameState game;

void game_init(void)
{
    game.in_menu = true;
    game.current_screen = SCREEN_FOYER;

    game_render_init();

    game.items[ITEM_1].texture_name = "item1";
    game.items[ITEM_1].held = false;
    game.items[ITEM_2].texture_name = "item2";
    game.items[ITEM_2].held = false;
    game.items[ITEM_3].texture_name = "item3";
    game.items[ITEM_3].held = false;

    // setting game state for testing
    //game.in_menu = false;
}

static void draw_texture(Texture2D tex, float x, float y, float w, float h)
{
    Rectangle src, dst;
    src = (Rectangle) { 0, 0, tex.width, tex.height };
    dst = (Rectangle) { x, y, w, h };
    DrawTexturePro(tex, src, dst, (Vector2) {0,0}, 0, (Color){255,255,255,255});
}

static void start_game(void)
{
    game.in_menu = false;
    game.menu_overlay = false;
}

static void end_game(void)
{
    game.in_menu = true;
}

static void render_menu_gui(void)
{
    int window_width = GetScreenWidth();
    Texture2D tex;
    bool pressed;
    float x;
    Timer* timer = &game.timers[TIMER_MENU_CAR];

    tex = get_texture_from_config("car");

    if (!timer->set) {
        x = (window_width-tex.width)/2;
    } else {
        x = lerp((window_width-tex.width)/2, window_width, timer->value, timer->max_value);
        if (timer->done) {
            timer_unset(TIMER_MENU_CAR);
            start_game();
        }
    }

    draw_texture(tex, x, 100, tex.width, tex.height);

    pressed = GuiButton(create_rect(window_width/2-50, 350, 100, 20), "#191#Play");
    if (pressed && !timer->set)
        timer_set(TIMER_MENU_CAR, 2.0f);

    pressed = GuiButton(create_rect(window_width/2-50, 390, 100, 20), "#191#Exit");
    if (pressed) {
        close_window_safely();
    }
}

static void render_menu_overlay(void)
{
    int window_width = GetScreenWidth();
    int window_height = GetScreenHeight();
    bool pressed;
    pressed = GuiButton(create_rect(window_width/2-50, window_height/2-50, 100, 20), "#191#Continue");
    if (pressed) {
        game.menu_overlay = false;
    }
    pressed = GuiButton(create_rect(window_width/2-50, window_height/2-10, 100, 20), "#191#Main Menu");
    if (pressed) {
        end_game();
    }
    pressed = GuiButton(create_rect(window_width/2-50, window_height/2+30, 100, 20), "#191#Exit");
    if (pressed) {
        close_window_safely();
    }
}

static void render_game_gui(void)
{
    int i;
    int offset = 0;
    int window_width = GetScreenWidth();
    int dim = 50;
    Item* item;
    Texture2D tex;
    for (i = 0; i < NUM_ITEMS; i++) {
        item = &game.items[i];
        if (!item->held) continue;
        tex = get_texture_from_config(item->texture_name);
        draw_texture(tex, window_width-dim-offset, 0, dim, dim);
        offset += 75;
    }

    game.screens[game.current_screen].render_gui();
    if (game.menu_overlay)
        render_menu_overlay();
}

static void render_game_objects(void)
{
    const char* background_texture_name = game.screens[game.current_screen].background_texture_name;
    Texture2D texture = get_texture_from_config(background_texture_name);
    draw_texture(texture, 0, 0, ctx.resolution.x, ctx.resolution.y);
    game.screens[game.current_screen].render();
}

static void key_callback(void)
{
    if (!game.in_menu && IsKeyPressed(KEY_ESCAPE)) {
        game.menu_overlay = !game.menu_overlay;
    }
}

void screen_transition(ScreenEnum screen)
{
    game.current_screen = screen;
}

void timer_set(TimerEnum timer, float max_value)
{
    game.timers[timer].value = 0;
    game.timers[timer].max_value = max_value;
    game.timers[timer].active = true;
    game.timers[timer].set = true;
    game.timers[timer].done = false;
}

void timer_unset(TimerEnum timer)
{
    game.timers[timer].value = 0;
    game.timers[timer].max_value = 0;
    game.timers[timer].active = false;
    game.timers[timer].set = false;
    game.timers[timer].done = false;
}

void game_update(float dt)
{
    for (int i = 0; i < NUM_TIMERS; i++) {
        if (game.timers[i].active) {
            game.timers[i].value += dt;
            if (game.timers[i].value > game.timers[i].max_value) {
                game.timers[i].value = game.timers[i].max_value;
                game.timers[i].active = false;
                game.timers[i].done = true;
            }
        }
    }

    key_callback();
}

void game_render(void)
{
    if (!game.in_menu)
        render_game_objects();
}

void game_render_gui(void)
{
    if (game.in_menu)
        render_menu_gui();
    else
        render_game_gui();
}

void game_cleanup(void)
{
}
