#include <stdlib.h>
#include "game.h"
#include "main.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// persistent game state among screens

GameState game;

static void render_attic(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go To Master Bedroom");
    if (pressed) {
        game.current_screen = SCREEN_MASTER_BEDROOM;
    }
}

static void render_bathroom(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go To Hallway");
    if (pressed) {
        game.current_screen = SCREEN_HALLWAY;
    }
}

static void render_guest_bedroom(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go To Hallway");
    if (pressed) {
        game.current_screen = SCREEN_HALLWAY;
    }
}

static void render_hallway(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go To Foyer");
    if (pressed) {
        game.current_screen = SCREEN_FOYER;
    }
    pressed = GuiButton((Rectangle){ 24, 60, 120, 30 }, "#191#Go To Guest Room");
    if (pressed) {
        game.current_screen = SCREEN_GUEST_BEDROOM;
    }
    pressed = GuiButton((Rectangle){ 24, 96, 120, 30 }, "#191#Go To Master Bedroom");
    if (pressed) {
        game.current_screen = SCREEN_MASTER_BEDROOM;
    }
    pressed = GuiButton((Rectangle){ 24, 132, 120, 30 }, "#191#Go To Bathroom");
    if (pressed) {
        game.current_screen = SCREEN_BATHROOM;
    }
}

static void render_master_bedroom(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go To Hallway");
    if (pressed) {
        game.current_screen = SCREEN_HALLWAY;
    }
    pressed = GuiButton((Rectangle){ 24, 60, 120, 30 }, "#191#Go To Attic");
    if (pressed) {
        game.current_screen = SCREEN_ATTIC;
    }
}

static void render_basement(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go To Kitchen");
    if (pressed) {
        game.current_screen = SCREEN_KITCHEN;
    }
}

static void render_kitchen(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go To Foyer");
    if (pressed) {
        game.current_screen = SCREEN_FOYER;
    }
    pressed = GuiButton((Rectangle){ 24, 60, 120, 30 }, "#191#Go To Basement");
    if (pressed) {
        game.current_screen = SCREEN_BASEMENT;
    }
    pressed = GuiButton((Rectangle){ 24, 96, 120, 30 }, "#191#Go To Dining Room");
    if (pressed) {
        game.current_screen = SCREEN_DINING_ROOM;
    }
}

static void render_dining(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go To Living Room");
    if (pressed) {
        game.current_screen = SCREEN_LIVING_ROOM;
    }
    pressed = GuiButton((Rectangle){ 24, 60, 120, 30 }, "#191#Go To Kitchen");
    if (pressed) {
        game.current_screen = SCREEN_KITCHEN;
    }
}

static void render_foyer(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go To Hallway");
    if (pressed) {
        game.current_screen = SCREEN_HALLWAY;
    }
    pressed = GuiButton((Rectangle){ 24, 60, 120, 30 }, "#191#Go To Kitchen");
    if (pressed) {
        game.current_screen = SCREEN_KITCHEN;
    }
    pressed = GuiButton((Rectangle){ 24, 96, 120, 30 }, "#191#Go To Living Room");
    if (pressed) {
        game.current_screen = SCREEN_LIVING_ROOM;
    }
}

static void render_living_room(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Go To Foyer");
    if (pressed) {
        game.current_screen = SCREEN_FOYER;
    }
    pressed = GuiButton((Rectangle){ 24, 60, 120, 30 }, "#191#Go To Dining Room");
    if (pressed) {
        game.current_screen = SCREEN_DINING_ROOM;
    }
}

void game_init(void)
{
    int i;
    game.in_menu = true;
    game.current_screen = SCREEN_FOYER;

    game.screens[SCREEN_LIVING_ROOM].background_texture_name = "living_room";
    game.screens[SCREEN_LIVING_ROOM].render = render_living_room;

    game.screens[SCREEN_FOYER].background_texture_name = "foyer";
    game.screens[SCREEN_FOYER].render = render_foyer;

    game.screens[SCREEN_DINING_ROOM].background_texture_name = "dining";
    game.screens[SCREEN_DINING_ROOM].render = render_dining;

    game.screens[SCREEN_KITCHEN].background_texture_name = "kitchen";
    game.screens[SCREEN_KITCHEN].render = render_kitchen;

    game.screens[SCREEN_BASEMENT].background_texture_name = "basement";
    game.screens[SCREEN_BASEMENT].render = render_basement;

    game.screens[SCREEN_MASTER_BEDROOM].background_texture_name = "master_bedroom";
    game.screens[SCREEN_MASTER_BEDROOM].render = render_master_bedroom;

    game.screens[SCREEN_HALLWAY].background_texture_name = "hallway";
    game.screens[SCREEN_HALLWAY].render = render_hallway;

    game.screens[SCREEN_GUEST_BEDROOM].background_texture_name = "guest_bedroom";
    game.screens[SCREEN_GUEST_BEDROOM].render = render_guest_bedroom;

    game.screens[SCREEN_BATHROOM].background_texture_name = "bathroom";
    game.screens[SCREEN_BATHROOM].render = render_bathroom;

    game.screens[SCREEN_ATTIC].background_texture_name = "attic";
    game.screens[SCREEN_ATTIC].render = render_attic;

    for (i = 0; i < NUM_SCREENS; i++) {
        log_assert(game.screens[i].background_texture_name != NULL, "Missing background texture name for %d screen", i);
        log_assert(game.screens[i].render != NULL, "Missing render function for %d screen", i);
    }

    game.items[ITEM_1].texture_name = "item1";
    game.items[ITEM_1].held = false;
    game.items[ITEM_2].texture_name = "item2";
    game.items[ITEM_2].held = false;
    game.items[ITEM_3].texture_name = "item3";
    game.items[ITEM_3].held = false;
}

static void draw_texture(Texture2D tex, float x, float y, float w, float h)
{
    Rectangle src, dst;
    src = (Rectangle) { 0, 0, tex.width, tex.height };
    dst = (Rectangle) { x, y, w, h };
    DrawTexturePro(tex, src, dst, (Vector2) {0,0}, 0, (Color){255,255,255,255});
}

static void render_menu_stuff(void)
{
    int window_width = GetScreenWidth();
    //int window_height = GetScreenHeight();
    Texture2D tex;
    //Vector2 origin;
    bool pressed;

    tex = get_texture_from_config("placeholder");
    //origin = (Vector2) { window_width/2-100, window_height/2-100 };

    draw_texture(tex, window_width/2-250, 100, 500, 200);

    pressed = GuiButton((Rectangle){ window_width/2-50, 350, 100, 20}, "#191#Play");
    if (pressed) {
        game.in_menu = false;
    }
}

static void render_ui(void)
{
    int i;
    int offset = 0;
    int window_width = GetScreenWidth();
    int dim = 50;
    Texture2D tex;
    for (i = 0; i < NUM_ITEMS; i++) {
        tex = get_texture_from_config(game.items[i].texture_name);
        draw_texture(tex, window_width-dim-offset, 0, dim, dim);
        offset += 75;
    }
}

static void render_game_stuff(void)
{
    ScreenRenderFuncPtr render_screen = game.screens[game.current_screen].render;
    const char* background_texture_name = game.screens[game.current_screen].background_texture_name;
    Texture2D texture = get_texture_from_config(background_texture_name);
    draw_texture(texture, 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight());
    render_screen();
    render_ui();
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
}
