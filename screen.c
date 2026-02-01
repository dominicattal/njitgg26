#include <stdlib.h>
#include "game.h"
#include "main.h"
#include "raygui.h"
#include "config.h"

#define ROOM_HITBOX_COLOR ((Color){255,255,0,100})
#define ITEM_HITBOX_COLOR ((Color){255,0,255,100})
#define CHARACTER_HITBOX_COLOR ((Color){0,255,255,100})
#define MISC_HITBOX_COLOR ((Color){255,0,0,100})

static bool check_collision_and_valid(Vector2 pos, Rectangle rect)
{
    if (in_dialogue()) return false;
    if (game.queried_item != ITEM_NONE) return false;
    //if (timer_isset(TIMER_SCREEN_TRANSITION)) return false;
    return CheckCollisionPointRec(pos, rect);
}

static void render_attic(void)
{
    Rectangle hitbox;
    Vector2 mouse_position;
    hitbox = create_rect2(275, 557, 550, 760);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_MASTER_BEDROOM);
    }
}

static void render_bathroom(void)
{
    Rectangle hitbox;
    Vector2 mouse_position;
    hitbox = create_rect2(1000, 1000, 1300, 1080);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_HALLWAY);
    }
}

static void render_guest_bedroom(void)
{
    Rectangle hitbox;
    Vector2 mouse_position;
    hitbox = create_rect2(1700, 300, 1750, 670);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_HALLWAY);
    }
}

static void render_hallway(void)
{
    Rectangle hitbox;
    Vector2 mouse_position;
    hitbox = create_rect2(70, 330, 235, 615);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_GUEST_BEDROOM);
    }
    hitbox = create_rect2(730, 60, 1000, 373);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_BATHROOM);
    }
    hitbox = create_rect2(1670, 250, 1756, 600);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_MASTER_BEDROOM);
    }
    hitbox = create_rect2(730, 1000, 1030, 1080);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_FOYER);
    }
}

static void render_master_bedroom(void)
{
    Rectangle hitbox;
    Vector2 mouse_position;
    hitbox = create_rect2(450, 1000, 750, 1080);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_HALLWAY);
    }
    hitbox = create_rect2(202, 418, 450, 700);
    if (get_flag(FLAG_OPENED_ATTIC_DOOR)) {
        mouse_position = get_scaled_mouse_position();
        DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
        if (check_collision_and_valid(mouse_position, hitbox)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                screen_transition(SCREEN_ATTIC);
        }
    } else {
        mouse_position = get_scaled_mouse_position();
        DrawRectangleRec(hitbox, MISC_HITBOX_COLOR);
        if (check_collision_and_valid(mouse_position, hitbox)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (selected_item(ITEM_FOYER)) {
                    set_flag(FLAG_OPENED_ATTIC_DOOR, true);
                    take_item(ITEM_FOYER);
                } else {
                    create_dialogue(CROW, "I need a key to open this");
                }
            }
        }
    }
}

static void render_basement(void)
{
    Rectangle hitbox;
    Vector2 mouse_position;
    hitbox = create_rect2(0, 200, 270, 612);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_KITCHEN);
    }
}

static void render_kitchen(void)
{
    Rectangle hitbox;
    Vector2 mouse_position;
    hitbox = create_rect2(111, 400, 243, 700);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_FOYER);
    }
    hitbox = create_rect2(1534, 70, 1792, 403);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_BASEMENT);
    }
    hitbox = create_rect2(329, 978, 619, 1080);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_DINING_ROOM);
    }
    hitbox = render_character(PIG, 448, 664);
}

static void render_dining(void)
{
    Rectangle hitbox;
    Vector2 mouse_position;
    hitbox = create_rect2(370, 150, 600, 490);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_KITCHEN);
    }
    hitbox = create_rect2(13, 391, 186, 682);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_LIVING_ROOM);
    }
}

static void render_foyer(void)
{
    Rectangle hitbox = create_rect2(636, 428, 945, 862);
    Vector2 mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_HALLWAY);
    }
    hitbox = create_rect2(1800, 519, 1920, 1000);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_KITCHEN);
    }
    hitbox = create_rect2(427, 990, 800, 1080);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_LIVING_ROOM);
    }
    if (!get_flag(FLAG_PICKED_UP_FOYER_ITEM)) {
        hitbox = create_rect2(1576, 690, 1606, 719);
        mouse_position = get_scaled_mouse_position();
        DrawRectangleRec(hitbox, ITEM_HITBOX_COLOR);
        if (check_collision_and_valid(mouse_position, hitbox)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                play_sound("interact");
                set_flag(FLAG_PICKED_UP_FOYER_ITEM, true);
                give_item(ITEM_FOYER);
            }
        }
    }
    mouse_position = get_scaled_mouse_position();
    //hitbox = create_rect2(1183, 523, 1400, 825);
    //DrawRectangleRec(hitbox, CHARACTER_HITBOX_COLOR);
    hitbox = render_character(BEAR, 1289, 870);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(BEAR, get_text_from_config("bear_crow_intro_1"));
            create_dialogue(CROW, get_text_from_config("bear_crow_intro_2"));
        }
    }
}

static void render_living_room(void)
{
    Rectangle hitbox;
    Vector2 mouse_position;
    hitbox = create_rect2(112, 185, 381, 483);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_FOYER);
    }
    hitbox = create_rect2(1700, 330, 1792, 798);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_DINING_ROOM);
    }
}

static void render_attic_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "Go To Master Bedroom");
    if (pressed) {
        screen_transition(SCREEN_MASTER_BEDROOM);
    }
}

static void render_bathroom_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "Go To Hallway");
    if (pressed) {
        screen_transition(SCREEN_HALLWAY);
    }
}

static void render_guest_bedroom_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "Go To Hallway");
    if (pressed) {
        screen_transition(SCREEN_HALLWAY);
    }
}

static void render_hallway_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "Go To Foyer");
    if (pressed) {
        screen_transition(SCREEN_FOYER);
    }
    pressed = GuiButton((Rectangle){ 24, 84, 120, 50 }, "Go To Guest Room");
    if (pressed) {
        screen_transition(SCREEN_GUEST_BEDROOM);
    }
    pressed = GuiButton((Rectangle){ 24, 144, 120, 50 }, "Go To Master Bedroom");
    if (pressed) {
        screen_transition(SCREEN_MASTER_BEDROOM);
    }
    pressed = GuiButton((Rectangle){ 24, 204, 120, 50 }, "Go To Bathroom");
    if (pressed) {
        screen_transition(SCREEN_BATHROOM);
    }
}

static void render_master_bedroom_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "Go To Hallway");
    if (pressed) {
        screen_transition(SCREEN_HALLWAY);
    }
    pressed = GuiButton((Rectangle){ 24, 84, 120, 50 }, "Go To Attic");
    if (pressed) {
        screen_transition(SCREEN_ATTIC);
    }
}

static void render_basement_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "Go To Kitchen");
    if (pressed) {
        screen_transition(SCREEN_KITCHEN);
    }
}

static void render_kitchen_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "Go To Foyer");
    if (pressed) {
        screen_transition(SCREEN_FOYER);
    }
    pressed = GuiButton((Rectangle){ 24, 84, 120, 50 }, "Go To Basement");
    if (pressed) {
        screen_transition(SCREEN_BASEMENT);
    }
    pressed = GuiButton((Rectangle){ 24, 144, 120, 50 }, "Go To Dining Room");
    if (pressed) {
        screen_transition(SCREEN_DINING_ROOM);
    }
}

static void render_dining_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "Go To Living Room");
    if (pressed) {
        screen_transition(SCREEN_LIVING_ROOM);
    }
    pressed = GuiButton((Rectangle){ 24, 84, 120, 50 }, "Go To Kitchen");
    if (pressed) {
        screen_transition(SCREEN_KITCHEN);
    }
}

static void render_foyer_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "Go To Hallway");
    if (pressed) {
        screen_transition(SCREEN_HALLWAY);
    }
    pressed = GuiButton((Rectangle){ 24, 84, 120, 50 }, "Go To Kitchen");
    if (pressed) {
        screen_transition(SCREEN_KITCHEN);
    }
    pressed = GuiButton((Rectangle){ 24, 144, 120, 50 }, "Go To Living Room");
    if (pressed) {
        screen_transition(SCREEN_LIVING_ROOM);
    }
}

static void render_living_room_gui(void)
{
    bool pressed;
    pressed = GuiButton((Rectangle){ 24, 24, 120, 50 }, "Go To Foyer");
    if (pressed) {
        screen_transition(SCREEN_FOYER);
    }
    pressed = GuiButton((Rectangle){ 24, 84, 120, 50 }, "Go To Dining Room");
    if (pressed) {
        screen_transition(SCREEN_DINING_ROOM);
    }
}

void screen_init(void)
{
    game.screens[SCREEN_LIVING_ROOM].background_texture_name = "living_room";
    game.screens[SCREEN_LIVING_ROOM].render = render_living_room;
    //game.screens[SCREEN_LIVING_ROOM].render_gui = render_living_room_gui;

    game.screens[SCREEN_FOYER].background_texture_name = "foyer";
    game.screens[SCREEN_FOYER].render = render_foyer;
    //game.screens[SCREEN_FOYER].render_gui = render_foyer_gui;

    game.screens[SCREEN_DINING_ROOM].background_texture_name = "dining";
    game.screens[SCREEN_DINING_ROOM].render = render_dining;
    //game.screens[SCREEN_DINING_ROOM].render_gui = render_dining_gui;

    game.screens[SCREEN_KITCHEN].background_texture_name = "kitchen";
    game.screens[SCREEN_KITCHEN].render = render_kitchen;
    //game.screens[SCREEN_KITCHEN].render_gui = render_kitchen_gui;

    game.screens[SCREEN_BASEMENT].background_texture_name = "basement";
    game.screens[SCREEN_BASEMENT].render = render_basement;
    //game.screens[SCREEN_BASEMENT].render_gui = render_basement_gui;

    game.screens[SCREEN_MASTER_BEDROOM].background_texture_name = "master_bedroom";
    game.screens[SCREEN_MASTER_BEDROOM].render = render_master_bedroom;
    //game.screens[SCREEN_MASTER_BEDROOM].render_gui = render_master_bedroom_gui;

    game.screens[SCREEN_HALLWAY].background_texture_name = "hallway";
    game.screens[SCREEN_HALLWAY].render = render_hallway;
    //game.screens[SCREEN_HALLWAY].render_gui = render_hallway_gui;

    game.screens[SCREEN_GUEST_BEDROOM].background_texture_name = "guest_bedroom";
    game.screens[SCREEN_GUEST_BEDROOM].render = render_guest_bedroom;
    //game.screens[SCREEN_GUEST_BEDROOM].render_gui = render_guest_bedroom_gui;

    game.screens[SCREEN_BATHROOM].background_texture_name = "bathroom";
    game.screens[SCREEN_BATHROOM].render = render_bathroom;
    //game.screens[SCREEN_BATHROOM].render_gui = render_bathroom_gui;

    game.screens[SCREEN_ATTIC].background_texture_name = "attic";
    game.screens[SCREEN_ATTIC].render = render_attic;
    //game.screens[SCREEN_ATTIC].render_gui = render_attic_gui;

    for (int i = 0; i < NUM_SCREENS; i++) {
        if (game.screens[i].background_texture_name == NULL)
            TraceLog(LOG_FATAL, "Missing background texture name for %d screen", i);
        if (game.screens[i].render == NULL)
            TraceLog(LOG_FATAL, "Missing render function for %d screen", i);
    }
}
