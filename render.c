#include <stdlib.h>
#include "game.h"
#include "main.h"
#include "raygui.h"

static void render_attic(void)
{
}

static void render_bathroom(void)
{
}

static void render_guest_bedroom(void)
{
}

static void render_hallway(void)
{
}

static void render_master_bedroom(void)
{
}

static void render_basement(void)
{
}

static void render_kitchen(void)
{
}

static void render_dining(void)
{
}

static void render_foyer(void)
{
}

static void render_living_room(void)
{
}

static void render_attic_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "#191#Go To Master Bedroom");
    if (pressed) {
        game.current_screen = SCREEN_MASTER_BEDROOM;
    }
}

static void render_bathroom_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "#191#Go To Hallway");
    if (pressed) {
        game.current_screen = SCREEN_HALLWAY;
    }
}

static void render_guest_bedroom_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "#191#Go To Hallway");
    if (pressed) {
        game.current_screen = SCREEN_HALLWAY;
    }
}

static void render_hallway_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "#191#Go To Foyer");
    if (pressed) {
        game.current_screen = SCREEN_FOYER;
    }
    pressed = GuiButton((Rectangle){ 24, 84, 120, 50 }, "#191#Go To Guest Room");
    if (pressed) {
        game.current_screen = SCREEN_GUEST_BEDROOM;
    }
    pressed = GuiButton((Rectangle){ 24, 144, 120, 50 }, "#191#Go To Master Bedroom");
    if (pressed) {
        game.current_screen = SCREEN_MASTER_BEDROOM;
    }
    pressed = GuiButton((Rectangle){ 24, 204, 120, 50 }, "#191#Go To Bathroom");
    if (pressed) {
        game.current_screen = SCREEN_BATHROOM;
    }
}

static void render_master_bedroom_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "#191#Go To Hallway");
    if (pressed) {
        game.current_screen = SCREEN_HALLWAY;
    }
    pressed = GuiButton((Rectangle){ 24, 84, 120, 50 }, "#191#Go To Attic");
    if (pressed) {
        game.current_screen = SCREEN_ATTIC;
    }
}

static void render_basement_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "#191#Go To Kitchen");
    if (pressed) {
        game.current_screen = SCREEN_KITCHEN;
    }
}

static void render_kitchen_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "#191#Go To Foyer");
    if (pressed) {
        game.current_screen = SCREEN_FOYER;
    }
    pressed = GuiButton((Rectangle){ 24, 84, 120, 50 }, "#191#Go To Basement");
    if (pressed) {
        game.current_screen = SCREEN_BASEMENT;
    }
    pressed = GuiButton((Rectangle){ 24, 144, 120, 50 }, "#191#Go To Dining Room");
    if (pressed) {
        game.current_screen = SCREEN_DINING_ROOM;
    }
}

static void render_dining_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "#191#Go To Living Room");
    if (pressed) {
        game.current_screen = SCREEN_LIVING_ROOM;
    }
    pressed = GuiButton((Rectangle){ 24, 84, 120, 50 }, "#191#Go To Kitchen");
    if (pressed) {
        game.current_screen = SCREEN_KITCHEN;
    }
}

static void render_foyer_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "#191#Go To Hallway");
    if (pressed) {
        game.current_screen = SCREEN_HALLWAY;
    }
    pressed = GuiButton((Rectangle){ 24, 84, 120, 50 }, "#191#Go To Kitchen");
    if (pressed) {
        game.current_screen = SCREEN_KITCHEN;
    }
    pressed = GuiButton((Rectangle){ 24, 144, 120, 50 }, "#191#Go To Living Room");
    if (pressed) {
        game.current_screen = SCREEN_LIVING_ROOM;
    }
}

static void render_living_room_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "#191#Go To Foyer");
    if (pressed) {
        game.current_screen = SCREEN_FOYER;
    }
    pressed = GuiButton((Rectangle){ 24, 84, 120, 50 }, "#191#Go To Dining Room");
    if (pressed) {
        game.current_screen = SCREEN_DINING_ROOM;
    }
}

void game_render_init(void)
{
    game.screens[SCREEN_LIVING_ROOM].background_texture_name = "living_room";
    game.screens[SCREEN_LIVING_ROOM].render = render_living_room;
    game.screens[SCREEN_LIVING_ROOM].render_gui = render_living_room_gui;

    game.screens[SCREEN_FOYER].background_texture_name = "foyer";
    game.screens[SCREEN_FOYER].render = render_foyer;
    game.screens[SCREEN_FOYER].render_gui = render_foyer_gui;

    game.screens[SCREEN_DINING_ROOM].background_texture_name = "dining";
    game.screens[SCREEN_DINING_ROOM].render = render_dining;
    game.screens[SCREEN_DINING_ROOM].render_gui = render_dining_gui;

    game.screens[SCREEN_KITCHEN].background_texture_name = "kitchen";
    game.screens[SCREEN_KITCHEN].render = render_kitchen;
    game.screens[SCREEN_KITCHEN].render_gui = render_kitchen_gui;

    game.screens[SCREEN_BASEMENT].background_texture_name = "basement";
    game.screens[SCREEN_BASEMENT].render = render_basement;
    game.screens[SCREEN_BASEMENT].render_gui = render_basement_gui;

    game.screens[SCREEN_MASTER_BEDROOM].background_texture_name = "master_bedroom";
    game.screens[SCREEN_MASTER_BEDROOM].render = render_master_bedroom;
    game.screens[SCREEN_MASTER_BEDROOM].render_gui = render_master_bedroom_gui;

    game.screens[SCREEN_HALLWAY].background_texture_name = "hallway";
    game.screens[SCREEN_HALLWAY].render = render_hallway;
    game.screens[SCREEN_HALLWAY].render_gui = render_hallway_gui;

    game.screens[SCREEN_GUEST_BEDROOM].background_texture_name = "guest_bedroom";
    game.screens[SCREEN_GUEST_BEDROOM].render = render_guest_bedroom;
    game.screens[SCREEN_GUEST_BEDROOM].render_gui = render_guest_bedroom_gui;

    game.screens[SCREEN_BATHROOM].background_texture_name = "bathroom";
    game.screens[SCREEN_BATHROOM].render = render_bathroom;
    game.screens[SCREEN_BATHROOM].render_gui = render_bathroom_gui;

    game.screens[SCREEN_ATTIC].background_texture_name = "attic";
    game.screens[SCREEN_ATTIC].render = render_attic;
    game.screens[SCREEN_ATTIC].render_gui = render_attic_gui;

    for (int i = 0; i < NUM_SCREENS; i++) {
        if (game.screens[i].background_texture_name == NULL)
            TraceLog(LOG_FATAL, "Missing background texture name for %d screen", i);
        if (game.screens[i].render == NULL)
            TraceLog(LOG_FATAL, "Missing render function for %d screen", i);
        if (game.screens[i].render_gui == NULL)
            TraceLog(LOG_FATAL, "Missing render gui function for %d screen", i);
    }
}
