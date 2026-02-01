#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "main.h"
#include "raygui.h"
#include "config.h"

#define ROOM_HITBOX_COLOR ((Color){255,255,0,100})
#define ITEM_HITBOX_COLOR ((Color){255,0,255,100})
#define CHARACTER_HITBOX_COLOR ((Color){0,255,255,100})
#define MISC_HITBOX_COLOR ((Color){255,0,0,100})

bool check_collision_and_valid(Vector2 pos, Rectangle rect)
{
    if (in_dialogue()) return false;
    if (game.queried_item != ITEM_NONE) return false;
    if (get_flag(IN_TRANSITION)) return false;
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
    Texture2D tex;
    Rectangle hitbox, hitbox2;
    Vector2 mouse_position = get_scaled_mouse_position();
    hitbox = create_rect2(1700, 300, 1750, 670);
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            screen_transition(SCREEN_HALLWAY);
    }

    tex = get_texture_from_config("bookshelf");
    hitbox = draw_texture_def(tex, 1263, 600);
    tex = get_texture_from_config("book");
    hitbox2 = hitbox_from_hitbox(hitbox, 233-11, 185-18, 233+tex.width, 185+tex.height);
    draw_texture_rect(tex, hitbox2);
    if (check_collision_and_valid(mouse_position, hitbox2)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("filler_book2"));
        }
    } else if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("filler_book1"));
        }
    }

    tex = get_texture_from_config("desk_with_drawers");
    hitbox = draw_texture_def(tex, 814, 600);
    hitbox2 = hitbox_from_hitbox(hitbox, 34, 37, 128, 123);
    DrawRectangleRec(hitbox2, MISC_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("filler_desk1"));
        }
    }
    hitbox2 = hitbox_from_hitbox(hitbox, 154, 40, 297, 123);
    DrawRectangleRec(hitbox2, MISC_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("filler_desk1"));
        }
    }
    hitbox2 = hitbox_from_hitbox(hitbox, 324, 38, 417, 123);
    DrawRectangleRec(hitbox2, MISC_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("filler_desk1"));
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
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (game.act == ACT2 && !get_flag(KNOCKED_ON_BATHROOM_DOOR)) {
                if (get_flag(TALKED_TO_FISH)) {
                    set_flag(KNOCKED_ON_BATHROOM_DOOR, true);
                    create_dialogue(CROW, "Is someone in there?");
                    create_dialogue(BEAR, "Be right out");
                } else {
                    create_dialogue(CROW, get_text_from_config("crow_try_bathroom"));
                }
            } else {
                screen_transition(SCREEN_BATHROOM);
            }
        }
    }
    hitbox = create_rect2(1670, 250, 1756, 600);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if ((!get_flag(TALKED_TO_ALL) || !get_flag(TALKED_TO_BEAR)) && game.act == ACT1)
                create_dialogue(CROW, "I probably shouldn't go into my parent's room");
            else
                screen_transition(SCREEN_MASTER_BEDROOM);
        }
    }
    hitbox = create_rect2(730, 1000, 1030, 1080);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (game.act == ACT2) {
                if (!get_flag(TALKED_TO_FISH))
                    screen_transition(SCREEN_FOYER);
                else if (!get_flag(KNOCKED_ON_BATHROOM_DOOR))
                    create_dialogue(CROW, "I should go find my father");
                else if (!get_flag(FINISHED_HALLWAY_CONVO))
                    create_dialogue(CROW, "I should ask my father if he's okay, he doesn't look well");
                else
                    screen_transition(SCREEN_FOYER);
            } else {
                screen_transition(SCREEN_FOYER);
            }
        }
    }
}

static void render_hallway_act2(void)
{
    Rectangle hitbox;
    Vector2 mouse_position = get_scaled_mouse_position();
    if (get_flag(FINISHED_BATHROOM_CONVO) && !get_flag(FINISHED_HALLWAY_CONVO)) {
        hitbox = render_character(BEAR, 681, 469);
        if (check_collision_and_valid(mouse_position, hitbox)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                set_flag(TALKED_TO_BEAR, true);
                create_dialogue(CROW, get_text_from_config("crow_bear_bathroom_1"));
                create_dialogue(BEAR, get_text_from_config("bear_crow_bathroom_2"));
                create_dialogue(CROW, get_text_from_config("crow_bear_bathroom_3"));
            }
        }
    }
    if (get_flag(FINISHED_POST_BATHROOM_CONVO) && !get_flag(FINISHED_HALLWAY_CONVO)) {
        hitbox = render_character(FISH, 1276, 829);
        if (check_collision_and_valid(mouse_position, hitbox)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                set_flag(TALKED_TO_FISH, true);
                create_dialogue(CROW, get_text_from_config("crow_bear_bathroom_1"));
                create_dialogue(BEAR, get_text_from_config("bear_crow_bathroom_2"));
                create_dialogue(CROW, get_text_from_config("crow_bear_bathroom_3"));
            }
        }
    }
}

static void render_master_bedroom(void)
{
    Rectangle hitbox, hitbox2;
    Vector2 mouse_position;
    Texture2D tex;
    hitbox = create_rect2(450, 1000, 750, 1080);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (game.act == ACT1 && !get_flag(PICKED_UP_BEAR_THING)) {
                create_dialogue(CROW, get_text_from_config("crow_try_leave_master_bedroom"));
            } else if (game.act == ACT2 && get_flag(FISH_WENT_TO_ROOM)) {
                create_dialogue(CROW, get_text_from_config("crow_try_to_leave_master_after_death"));
            } else {
                screen_transition(SCREEN_HALLWAY);
            }
        }
    }

    tex = get_texture_from_config("bookshelf");
    hitbox = draw_texture_def(tex, 1630, 627);
    tex = get_texture_from_config("book");
    hitbox2 = hitbox_from_hitbox(hitbox, 233-11, 185-18, 233+tex.width, 185+tex.height);
    if (!get_flag(PICKED_UP_BOOK))
        draw_texture_rect(tex, hitbox2);
    tex = get_texture_from_config("bookshelf");
    if (!get_flag(PICKED_UP_BOOK) && check_collision_and_valid(mouse_position, hitbox2)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            give_item(ITEM_BOOK);
            set_flag(PICKED_UP_BOOK, true);
            create_dialogue(CROW, get_text_from_config("get_important_book"));
        }
    } else if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("filler_book1"));
        }
    }

    tex = get_texture_from_config("desk_with_drawers");
    hitbox = draw_texture_def(tex, 751, 625);
    hitbox2 = hitbox_from_hitbox(hitbox, 34, 37, 128, 123);
    DrawRectangleRec(hitbox2, MISC_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox2)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("filler_desk1"));
        }
    }
    hitbox2 = hitbox_from_hitbox(hitbox, 154, 40, 297, 123);
    DrawRectangleRec(hitbox2, MISC_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox2)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("filler_desk1"));
        }
    }
    hitbox2 = hitbox_from_hitbox(hitbox, 324, 38, 417, 123);
    DrawRectangleRec(hitbox2, MISC_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox2)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (game.act == ACT1) {
                if (!get_flag(PICKED_UP_BEAR_NOTE)) {
                    set_flag(PICKED_UP_BEAR_NOTE, true);
                    give_item(ITEM_BEAR_NOTE);
                    create_dialogue(CROW, get_text_from_config("pickup_note"));
                } else if (!get_flag(BEAR_NOTE_RESPONSE)) {
                    create_dialogue(CROW, get_text_from_config("should_examine_note"));
                } else if (!get_flag(PICKED_UP_BEAR_THING)) {
                    set_flag(PICKED_UP_BEAR_THING, true);
                    give_item(ITEM_BEAR_THING);
                }
            }
        }
    }

    hitbox = create_rect2(1050, 920, 1450, 973);
    DrawRectangleRec(hitbox, MISC_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("under_master_bed"));
        }
    }

    hitbox = create_rect2(202, 418, 450, 700);
    if (get_flag(OPENED_ATTIC_DOOR)) {
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
                if (selected_item(ITEM_ATTIC_KEY)) {
                    set_flag(OPENED_ATTIC_DOOR, true);
                    take_item(ITEM_ATTIC_KEY);
                } else {
                    create_dialogue(CROW, "I need a key to open this");
                }
            }
        }
    }
}

static void render_master_bedroom_act2(void)
{
    Rectangle hitbox;
    Texture2D tex;
    Vector2 mouse_position = get_scaled_mouse_position();
    if (get_flag(FISH_WENT_TO_ROOM)) {
        hitbox = render_character(FISH, 501, 768);
        if (check_collision_and_valid(mouse_position, hitbox)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                set_flag(TALKED_TO_FISH, true);
                create_dialogue(FISH, get_text_from_config("fish_master_bedroom_1"));
            }
        }
        tex = get_texture_from_config("dead_bear");
        hitbox = draw_texture_def(tex, 796, 789);
        if (check_collision_and_valid(mouse_position, hitbox)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                set_flag(TALKED_TO_BEAR, true);
                create_dialogue(CROW, get_text_from_config("crow_dead_father"));
            }
        }
        if (get_flag(DOG_IN_MASTER_BEDROOM))
            hitbox = render_character(DOG, 1657, 736);
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

static void render_basement_act2(void)
{
    Rectangle hitbox;
    Vector2 mouse_position = get_scaled_mouse_position();
    hitbox = render_character(PIG, 1530, 710);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(TALKED_TO_PIG, true);
            create_dialogue(PIG, get_text_from_config("pig_crow_alcohol_1"));
            create_dialogue(CROW, get_text_from_config("crow_pig_alcohol_2"));
        }
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
    if (!get_flag(PICKED_UP_KNIFE) && game.act != ACT1) {
        hitbox = create_rect2(476, 214, 612, 318);
        DrawRectangleRec(hitbox, MISC_HITBOX_COLOR);
        if (check_collision_and_valid(mouse_position, hitbox)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                give_item(ITEM_KNIFE);
                set_flag(PICKED_UP_KNIFE, true);
                create_dialogue(CROW, get_text_from_config("crow_find_knife"));
            }
        }
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
            set_flag(TALKED_TO_PIG, true);
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
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (game.act == ACT2) {
                if (get_flag(FINISHED_HALLWAY_CONVO) && !get_flag(BEAR_WENT_TO_ROOM))
                    create_dialogue(CROW, get_text_from_config("shouldnt_exit_dining"));
                else
                    screen_transition(SCREEN_KITCHEN);
            } else {
                screen_transition(SCREEN_KITCHEN);
            }
        }
    }
    hitbox = create_rect2(13, 391, 186, 682);
    mouse_position = get_scaled_mouse_position();
    DrawRectangleRec(hitbox, ROOM_HITBOX_COLOR);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (game.act == ACT2) {
                if (get_flag(FINISHED_HALLWAY_CONVO) && !get_flag(BEAR_WENT_TO_ROOM))
                    create_dialogue(CROW, get_text_from_config("shouldnt_exit_dining"));
                else
                    screen_transition(SCREEN_LIVING_ROOM);
            } else {
                screen_transition(SCREEN_LIVING_ROOM);
            }
        }
    }
}

static void render_dining_act2(void)
{
    Rectangle hitbox;
    Vector2 mouse_position = get_scaled_mouse_position();
    int y = 871;
    hitbox = render_character(SNAKE, 900, y);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (get_flag(FINISHED_HALLWAY_CONVO)) {
                create_dialogue(SNAKE, get_text_from_config("snake_dining_2"));
                set_flag(TALKED_TO_SNAKE, true);
            } else {
                create_dialogue(SNAKE, get_text_from_config("snake_dining_1"));
                set_flag(TALKED_TO_CAT, true);
            }
        }
    }
    hitbox = render_character(CAT, 1200, y);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (get_flag(FINISHED_HALLWAY_CONVO)) {
                create_dialogue(CAT, get_text_from_config("cat_dining_2"));
                set_flag(TALKED_TO_CAT, true);
            } else {
                create_dialogue(CAT, get_text_from_config("cat_dining_1"));
                set_flag(TALKED_TO_CAT, true);
            }
        }
    }
    hitbox = render_character(OWL, 1500, y);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (get_flag(FINISHED_HALLWAY_CONVO)) {
                create_dialogue(OWL, get_text_from_config("owl_dining_2"));
                set_flag(TALKED_TO_OWL, true);
            } else {
                create_dialogue(OWL, get_text_from_config("owl_dining_1"));
                set_flag(TALKED_TO_OWL, true);
            }
        }
    }
    hitbox = render_character(DOG, 1634, 554);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (get_flag(FINISHED_HALLWAY_CONVO)) {
                create_dialogue(DOG, get_text_from_config("dog_dining_2"));
                set_flag(TALKED_TO_DOG, true);
            } else {
                create_dialogue(DOG, get_text_from_config("dog_dining_1"));
                set_flag(TALKED_TO_DOG, true);
            }
        }
    }
    if (!get_flag(FINISHED_HALLWAY_CONVO)) {
        hitbox = render_character(FISH, 436, 868);
        if (check_collision_and_valid(mouse_position, hitbox)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                set_flag(TALKED_TO_FISH, true);
                create_dialogue(FISH, get_text_from_config("fish_wonder_about_bear"));
            }
        }
    } else if (!get_flag(BEAR_WENT_TO_ROOM)) {
        hitbox = render_character(FISH, 570, y);
        if (check_collision_and_valid(mouse_position, hitbox)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                create_dialogue(FISH, get_text_from_config("fish_dining_1"));
                set_flag(TALKED_TO_FISH, true);
            }
        }
        hitbox = render_character(BEAR, 340, y);
        if (check_collision_and_valid(mouse_position, hitbox)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                create_dialogue(BEAR, get_text_from_config("bear_dining_1"));
                set_flag(TALKED_TO_BEAR, true);
            }
        }
    } else if (!get_flag(FISH_WENT_TO_ROOM)) {
        render_character(FISH, 570, y);
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
    if (!get_flag(PICKED_UP_FOYER_ITEM)) {
        hitbox = create_rect2(1576, 690, 1606, 719);
        mouse_position = get_scaled_mouse_position();
        DrawRectangleRec(hitbox, ITEM_HITBOX_COLOR);
        if (check_collision_and_valid(mouse_position, hitbox)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                play_sound("interact");
                set_flag(PICKED_UP_FOYER_ITEM, true);
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
            set_flag(TALKED_TO_BEAR, true);
            if (!get_flag(TALKED_TO_ALL)) {
                create_dialogue(BEAR, get_text_from_config("bear_crow_intro_1"));
                create_dialogue(CROW, get_text_from_config("crow_bear_intro_2"));
            } else if (!get_flag(BEAR_NOTE_RESPONSE)) {
                create_dialogue(BEAR, get_text_from_config("bear_crow_get_note_1"));
                create_dialogue(CROW, get_text_from_config("crow_bear_get_note_2"));
            } else if (!selected_item(ITEM_BEAR_THING)) {
                create_dialogue(CROW, get_text_from_config("crow_should_give_thing"));
            } else {
                take_item(ITEM_BEAR_THING);
                create_dialogue(BEAR, get_text_from_config("bear_receive_thing"));
                game.act_should_be = ACT2;
            }
        }
    }
    hitbox = render_character(FISH, 1024, 856);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(TALKED_TO_FISH, true);
            create_dialogue(FISH, get_text_from_config("fish_crow_intro_1"));
            create_dialogue(CROW, get_text_from_config("crow_fish_intro_2"));
        }
    }
    hitbox = render_character(OWL, 427, 881);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(TALKED_TO_OWL, true);
            create_dialogue(CROW, get_text_from_config("crow_owl_intro_1"));
            create_dialogue(OWL, get_text_from_config("owl_crow_intro_2"));
        }
    }
}

static void render_living_room(void)
{
    Rectangle hitbox;
    Vector2 mouse_position = get_scaled_mouse_position();
    Texture2D tex;
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
    tex = get_texture_from_config("bookshelf");
    hitbox = draw_texture_def(tex, 527, 506);
    if (check_collision_and_valid(mouse_position, hitbox) && !get_flag(BLAMING)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            create_dialogue(CROW, get_text_from_config("filler_book1"));
        }
    }
}

void render_living_room_act1(void)
{
    Rectangle hitbox;
    Vector2 mouse_position = get_scaled_mouse_position();
    hitbox = render_character(DOG, 1507, 585);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(TALKED_TO_DOG, true);
            create_dialogue(DOG, get_text_from_config("dog_crow_intro_1"));
        }
    }
    hitbox = render_character(SNAKE, 728, 571);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(TALKED_TO_SNAKE, true);
            create_dialogue(CROW, get_text_from_config("crow_snake_intro_1"));
            create_dialogue(SNAKE, get_text_from_config("snake_crow_intro_2"));
        }
    }
    hitbox = render_character(CAT, 1054, 588);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(TALKED_TO_CAT, true);
            create_dialogue(CROW, get_text_from_config("crow_cat_intro_1"));
            create_dialogue(CAT, get_text_from_config("crow_cat_intro_2"));
        }
    }
}

void render_living_room_act3(void)
{
    Rectangle hitbox;
    int y = 900;
    Vector2 mouse_position = get_scaled_mouse_position();
    hitbox = render_character(DOG, 234, y);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (get_flag(BLAMING))
                game.blamed_character = DOG;
            else
                create_dialogue(DOG, get_text_from_config("dog_act3"));
        }
    }
    hitbox = render_character(SNAKE, 504, y);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (get_flag(BLAMING))
                game.blamed_character = SNAKE;
            else
                create_dialogue(SNAKE, get_text_from_config("snake_act3"));
        }
    }
    hitbox = render_character(CAT, 773, y);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (get_flag(BLAMING))
                game.blamed_character = CAT;
            else
                create_dialogue(CAT, get_text_from_config("cat_act3"));
        }
    }
    hitbox = render_character(FISH, 1000, y);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (get_flag(BLAMING))
                game.blamed_character = FISH;
            else
                create_dialogue(FISH, get_text_from_config("fish_act3"));
        }
    }
    hitbox = render_character(PIG, 1300, y);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (get_flag(BLAMING))
                game.blamed_character = PIG;
            else
                create_dialogue(PIG, get_text_from_config("pig_act3"));
        }
    }
    hitbox = render_character(OWL, 1570, y);
    if (check_collision_and_valid(mouse_position, hitbox)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (get_flag(BLAMING))
                game.blamed_character = OWL;
            else
                create_dialogue(OWL, get_text_from_config("owl_act3"));
        }
    }
}

void screen_init(void)
{
    game.screens[SCREEN_LIVING_ROOM].background_texture_name = "living_room";
    game.screens[SCREEN_LIVING_ROOM].render = render_living_room;
    game.screens[SCREEN_LIVING_ROOM].render_act[ACT1] = render_living_room_act1;
    game.screens[SCREEN_LIVING_ROOM].render_act[ACT3] = render_living_room_act3;

    game.screens[SCREEN_FOYER].background_texture_name = "foyer";
    game.screens[SCREEN_FOYER].render = render_foyer;
    game.screens[SCREEN_FOYER].render_act[ACT1] = render_foyer_act1;

    game.screens[SCREEN_DINING_ROOM].background_texture_name = "dining";
    game.screens[SCREEN_DINING_ROOM].render = render_dining;
    game.screens[SCREEN_DINING_ROOM].render_act[ACT2] = render_dining_act2;

    game.screens[SCREEN_KITCHEN].background_texture_name = "kitchen";
    game.screens[SCREEN_KITCHEN].render = render_kitchen;
    game.screens[SCREEN_KITCHEN].render_act[ACT1] = render_kitchen_act1;

    game.screens[SCREEN_BASEMENT].background_texture_name = "basement";
    game.screens[SCREEN_BASEMENT].render = render_basement;
    game.screens[SCREEN_BASEMENT].render_act[ACT2] = render_basement_act2;

    game.screens[SCREEN_MASTER_BEDROOM].background_texture_name = "master_bedroom";
    game.screens[SCREEN_MASTER_BEDROOM].render = render_master_bedroom;
    game.screens[SCREEN_MASTER_BEDROOM].render_act[ACT2] = render_master_bedroom_act2;

    game.screens[SCREEN_HALLWAY].background_texture_name = "hallway";
    game.screens[SCREEN_HALLWAY].render = render_hallway;
    game.screens[SCREEN_HALLWAY].render_act[ACT2] = render_hallway_act2;

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
