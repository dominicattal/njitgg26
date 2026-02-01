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
    if (get_flag(FLAG_IN_TRANSITION)) return false;
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
    Vector2 mouse_position = get_scaled_mouse_position();
    hitbox = create_rect2(1700, 300, 1750, 670);
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_HALLWAY);
    }
    hitbox = create_rect2(700, 355, 820, 377);
    DrawRectangleRec(hitbox, MISC_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("missing_desk"));
        }
    }
    hitbox = create_rect2(700, 411, 820, 443);
    DrawRectangleRec(hitbox, MISC_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("missing_desk"));
        }
    }
    hitbox = create_rect2(701, 459, 818, 498);
    DrawRectangleRec(hitbox, MISC_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("missing_desk"));
        }
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
    mouse_position = get_scaled_mouse_position();
    hitbox = create_rect2(111, 400, 243, 700);
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_FOYER);
    }
    hitbox = create_rect2(1534, 70, 1792, 403);
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_BASEMENT);
    }
    hitbox = create_rect2(329, 978, 619, 1080);
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_DINING_ROOM);
    }
}

static void render_kitchen_act1(void)
{
    Rectangle hitbox;
    Vector2 mouse_position = get_scaled_mouse_position();
    hitbox = render_character(PIG, 448, 664);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(FLAG_TALKED_TO_PIG, true);
            create_dialogue(CROW, get_text_from_config("crow_pig_intro_1"));
            create_dialogue(PIG, get_text_from_config("crow_pig_intro_2"));
        }
    }
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
}

static void render_foyer_act1(void)
{
    Rectangle hitbox;
    Vector2 mouse_position = get_scaled_mouse_position();
    hitbox = render_character(BEAR, 1289, 870);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(FLAG_TALKED_TO_BEAR, true);
            if (!get_flag(FLAG_TALKED_TO_ALL)) {
                create_dialogue(BEAR, get_text_from_config("bear_crow_intro_1"));
                create_dialogue(CROW, get_text_from_config("crow_bear_intro_2"));
            } else {
                create_dialogue(BEAR, get_text_from_config("bear_crow_get_note_1"));
                create_dialogue(CROW, get_text_from_config("crow_bear_get_note_2"));
            }
        }
    }
    hitbox = render_character(FISH, 1024, 856);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(FLAG_TALKED_TO_FISH, true);
            create_dialogue(FISH, get_text_from_config("fish_crow_intro_1"));
            create_dialogue(CROW, get_text_from_config("crow_fish_intro_2"));
        }
    }
    hitbox = render_character(OWL, 427, 881);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(FLAG_TALKED_TO_OWL, true);
            create_dialogue(CROW, get_text_from_config("crow_owl_intro_1"));
            create_dialogue(OWL, get_text_from_config("owl_crow_intro_2"));
        }
    }
}

static void render_living_room(void)
{
    Rectangle hitbox;
    Vector2 mouse_position = get_scaled_mouse_position();
    hitbox = create_rect2(112, 185, 381, 483);
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_FOYER);
    }
    hitbox = create_rect2(1700, 330, 1792, 798);
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_DINING_ROOM);
    }
    hitbox = create_rect2(371, 133, 666, 520);
    DrawRectangleRec(hitbox, MISC_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("filler_book1"));
        }
    }
    hitbox = render_character(DOG, 1507, 585);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(FLAG_TALKED_TO_DOG, true);
            create_dialogue(DOG, get_text_from_config("dog_crow_intro_1"));
        }
    }
    hitbox = render_character(SNAKE, 728, 571);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(FLAG_TALKED_TO_SNAKE, true);
            create_dialogue(CROW, get_text_from_config("crow_snake_intro_1"));
            create_dialogue(SNAKE, get_text_from_config("snake_crow_intro_2"));
        }
    }
    hitbox = render_character(CAT, 1054, 588);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(FLAG_TALKED_TO_CAT, true);
            create_dialogue(CROW, get_text_from_config("crow_cat_intro_1"));
            create_dialogue(CAT, get_text_from_config("crow_cat_intro_2"));
        }
    }
}

void screen_init(void)
{
    game.screens[SCREEN_LIVING_ROOM].background_texture_name = "living_room";
    game.screens[SCREEN_LIVING_ROOM].render = render_living_room;

    game.screens[SCREEN_FOYER].background_texture_name = "foyer";
    game.screens[SCREEN_FOYER].render = render_foyer;
    game.screens[SCREEN_FOYER].render_act[ACT1] = render_foyer_act1;

    game.screens[SCREEN_DINING_ROOM].background_texture_name = "dining";
    game.screens[SCREEN_DINING_ROOM].render = render_dining;

    game.screens[SCREEN_KITCHEN].background_texture_name = "kitchen";
    game.screens[SCREEN_KITCHEN].render = render_kitchen;
    game.screens[SCREEN_KITCHEN].render_act[ACT1] = render_kitchen_act1;

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

    for (int i = 0; i < NUM_SCREENS; i++) {
        if (game.screens[i].background_texture_name == NULL)
            TraceLog(LOG_FATAL, "Missing background texture name for %d screen", i);
        if (game.screens[i].render == NULL)
            TraceLog(LOG_FATAL, "Missing render function for %d screen", i);
    }
}
