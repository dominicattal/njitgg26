#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "main.h"
#include "config.h"
#include "gui.h"
#include "raygui.h"

GameState game;

void set_flag(FlagEnum flag, bool val)
{
    game.flags[flag] = val;
}

bool toggle_flag(FlagEnum flag)
{
    return game.flags[flag] = !game.flags[flag];
}

bool get_flag(FlagEnum flag)
{
    return game.flags[flag];
}

void give_item(ItemEnum item)
{
    game.items[item].held = true;
}

void take_item(ItemEnum item)
{
    game.items[item].held = false;
}

bool has_item(ItemEnum item)
{
    return game.items[item].held;
}

bool selected_item(ItemEnum item)
{
    return game.selected_item == item;
}

char* character_display_name(CharacterEnum character)
{
    return game.characters[character].display_name;
}

void create_dialogue(CharacterEnum character, const char* dialogue)
{
    DialogueNode* node = malloc(sizeof(DialogueNode));
    node->character = character;
    node->dialogue = dialogue;
    node->next = NULL;
    if (game.dialogue_tail == NULL) {
        game.dialogue_head = node;
        game.dialogue_tail = node;
    } else {
        game.dialogue_tail->next = node;
        game.dialogue_tail = node;
    }
}

void advance_dialogue(void)
{
    if (game.dialogue_head == NULL) return;
    DialogueNode* node = game.dialogue_head;
    if (node == game.dialogue_tail)
        game.dialogue_head = game.dialogue_tail = NULL;
    else
        game.dialogue_head = node->next;
    free(node);
}

bool in_dialogue(void)
{
    return game.dialogue_head != NULL;
}

void act_transition(ActEnum act)
{
    for (FlagEnum i = TALKED_TO_BEAR; i <= TALKED_TO_OWL; i++)
        set_flag(i, false);
    game.act = act;
}

void game_init(void)
{
    set_flag(IN_MENU, true);
    game.current_screen = SCREEN_FOYER;
    game.dialogue_head = NULL;
    game.dialogue_tail = NULL;
    game.act_should_be = ACT_NONE;
    game.selected_item = ITEM_NONE;
    game.queried_item = ITEM_NONE;

    screen_init();
    item_init();
    character_init();

    // act1 testing
    // setting game state for testing
    //set_flag(IN_MENU, false);

    // set_flag(TALKED_TO_ALL, true);
    // set_flag(TALKED_TO_BEAR, true);
    // set_flag(PICKED_UP_BEAR_THING, true);
    // set_flag(BEAR_NOTE_RESPONSE, true);
    // give_item(ITEM_BEAR_THING);

    //act2 testing
    // game.act = ACT2;
    // game.current_screen = SCREEN_HALLWAY;
    // game.current_screen = SCREEN_ATTIC;
    // set_flag(KNOCKED_ON_BATHROOM_DOOR,true);
    // set_flag(FINISHED_BATHROOM_CONVO,true);
    // set_flag(FINISHED_POST_BATHROOM_CONVO,true);
    // set_flag(FINISHED_HALLWAY_CONVO,true);
    // set_flag(BEAR_ANNOUNCEMENT,true);
    // set_flag(BEAR_WENT_TO_ROOM,true);
    // set_flag(FISH_ANNOUNCEMENT,true);
    // set_flag(FISH_WENT_TO_ROOM, true);
    // give_item(ITEM_BEAR_NOTE);
    // give_item(ITEM_CHEST_KEY);

    // act3 testing
    //game.act = ACT3;
    //game.current_screen = SCREEN_LIVING_ROOM;
    //game.current_screen = NO_SCREEN;
    //for (int i = 0; i < NUM_ITEMS; i++)
    //    give_item(i);
}

void draw_texture(Texture2D tex, float x, float y, float w, float h)
{
    Rectangle src, dst;
    src = (Rectangle) { 0, 0, tex.width, tex.height };
    dst = (Rectangle) { x, y, w, h };
    DrawTexturePro(tex, src, dst, (Vector2) {0,0}, 0, (Color){255,255,255,255});
}

void draw_texture_rect(Texture2D tex, Rectangle rect)
{
    Rectangle src;
    src = (Rectangle) { 0, 0, tex.width, tex.height };
    DrawTexturePro(tex, src, rect, (Vector2) {0,0}, 0, (Color){255,255,255,255});
}

// return hitbox
Rectangle draw_texture_def(Texture2D tex, float x, float y)
{
    Rectangle src, dst;
    src = (Rectangle) { 0, 0, tex.width, tex.height };
    dst = (Rectangle) { x-tex.width/2, y-tex.height, tex.width, tex.height };
    DrawTexturePro(tex, src, dst, (Vector2) {0,0}, 0, (Color){255,255,255,255});
    return dst;
}
 
Rectangle hitbox_from_hitbox(Rectangle hitbox, float x1, float y1, float x2, float y2)
{
    Rectangle h;
    h.x = hitbox.x + x1;
    h.y = hitbox.y + y1;
    h.width = (hitbox.x + x2) - h.x;
    h.height = (hitbox.y + y2) - h.y;
    return h;
}

static void start_game(void)
{
    DialogueNode* next;
    DialogueNode* cur;
    cur = game.dialogue_head;
    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }
    game.dialogue_head = NULL;

    for (int flag = 0; flag < NUM_FLAGS; flag++)
        set_flag(flag, false);
    for (int item = 0; item < NUM_ITEMS; item++)
        game.items[item].held = false;

    game.current_screen = SCREEN_FOYER;
    game.selected_item = ITEM_NONE;
    game.queried_item = ITEM_NONE;
    game.act = ACT1;
}

static void end_game(void)
{
    set_flag(IN_MENU, true);
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

    pressed = GuiButton(create_rect(window_width/2-50, 350, 100, 20), "Play");
    if (pressed && !timer->set)
        timer_set(TIMER_MENU_CAR, 2.0f);

    pressed = GuiButton(create_rect(window_width/2-50, 390, 100, 20), "Exit");
    if (pressed) {
        close_window_safely();
    }
}

static void render_menu_overlay(void)
{
    int window_width = GetScreenWidth();
    int window_height = GetScreenHeight();
    bool pressed;
    pressed = GuiButton(create_rect(window_width/2-50, window_height/2-50, 100, 20), "Continue");
    if (pressed) {
        set_flag(MENU_OVERLAY, false);
    }
    pressed = GuiButton(create_rect(window_width/2-50, window_height/2-10, 100, 20), "Main Menu");
    if (pressed) {
        end_game();
    }
    pressed = GuiButton(create_rect(window_width/2-50, window_height/2+30, 100, 20), "Exit");
    if (pressed) {
        close_window_safely();
    }
}

static void render_game_gui(void)
{
    ItemEnum i;
    int offset_x = 35;
    int cur_offset_x = offset_x;
    int offset_y = 20;
    int dim = 50;
    Item* item;
    Texture2D tex;
    Rectangle rect;
    char* item_info = NULL;
    char* item_display_name = NULL;
    int num_held_items = 0;
    int window_width = GetScreenWidth();
    int window_height = GetScreenHeight();
    DialogueNode* node;
    Character* character;
    Vector2 mouse_position;

    mouse_position = GetMousePosition();

    for (i = 0; i < NUM_ITEMS; i++)
        if (game.items[i].held)
            num_held_items++;

    rect = create_rect(offset_x-10, offset_y-10, (num_held_items-1)*75+dim+20, dim+20);
    DrawRectangleRec(rect, (Color){120,120,120,255});
    for (i = 0; i < NUM_ITEMS; i++) {
        item = &game.items[i];
        if (!item->held) continue;
        tex = get_texture_from_config(item->texture_name);
        rect = create_rect(cur_offset_x, offset_y, dim, dim);
        if (CheckCollisionPointRec(GetMousePosition(), rect)) {
            item_info = get_text_from_config("item_foyer");
            item_display_name = item->display_name;
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (i == game.queried_item)
                    goto invalid_select;
                if (i == game.selected_item) 
                    game.selected_item = ITEM_NONE;
                else
                    game.selected_item = i;
            }
        invalid_select:
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                if (i == game.selected_item || item->render_query == NULL)
                    goto invalid_query;
                if (i == game.queried_item) 
                    game.queried_item = ITEM_NONE;
                else
                    game.queried_item = i;
            }
        }
        invalid_query:

        if (i == game.selected_item)
            DrawRectangleRec(rect, (Color){255,0,255,120});
        if (i == game.queried_item && !get_flag(BLAMING))
            DrawRectangleRec(rect, (Color){100,255,255,120});

        draw_texture_rect(tex, rect);
        cur_offset_x += 75;

        if (i == game.queried_item) {
            item->render_query();
        }
    }

    if (get_flag(MENU_OVERLAY))
        render_menu_overlay();

    if (item_info != NULL) {
        rect = create_rect(offset_x-10, offset_y+dim+10, 300, 30);
        DrawRectangleRec(rect, BLUE);
        DrawTextBoxed(get_font_from_config("consolas_16"), item_display_name, rect, 16, 0, true, WHITE);
        //rect = create_rect(offset_x-10, offset_y+dim+40, 300, 100);
        //DrawRectangleRec(rect, PURPLE);
        //DrawTextBoxed(get_font_from_config("consolas_16"), item_info, rect, 16, 0, true, WHITE);
        //DrawText(item_info, window_width-300, dim, 20, BLACK);
    }

    // total width = 960
    const int TOTAL_WIDTH = 960;
    if (in_dialogue()) {
        node = game.dialogue_head;
        character = &game.characters[node->character];

        tex = get_texture_from_config(character->portrait_texture_name);
        rect = create_rect((window_width-TOTAL_WIDTH)/2, window_height-220, 200, 200);
        DrawRectangleRec(rect, ORANGE);
        draw_texture_rect(tex, rect);

        rect = create_rect((window_width-TOTAL_WIDTH)/2+200, window_height-220, 960-200, 30);
        DrawRectangleRec(rect, MAROON);
        DrawTextBoxed(get_font_from_config("consolas_16"), character->display_name, rect, 16, 0, true, WHITE);

        rect = create_rect((window_width-TOTAL_WIDTH)/2+200, window_height-190, 960-200, 170);
        DrawRectangleRec(rect, DARKGRAY);
        DrawTextBoxed(get_font_from_config("consolas_16"), node->dialogue, rect, 16, 0, true, WHITE);

        tex = get_texture_from_config("right_arrow");
        rect = create_rect((window_width-TOTAL_WIDTH)/2+TOTAL_WIDTH-50, window_height-220, 50, 30);
        draw_texture_rect(tex, rect);
        if (CheckCollisionPointRec(mouse_position, rect)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                advance_dialogue();
        }
    }

    Vector2 pos;
    char buf[128];
    if (game.act == ACT3 && game.current_screen == SCREEN_LIVING_ROOM) {
        if (get_flag(BLAMING) && game.blamed_character != NO_CHARACTER) {
            pos = (Vector2) {(window_width-400)/2, 100};
            rect = create_rect((window_width-400)/2, 100, 400, 100);
            sprintf(buf, "Blame the %s?", game.characters[game.blamed_character].display_name);
            DrawRectangleRec(rect, BROWN);
            DrawTextEx(get_font_from_config("consolas_32"), buf, pos, 32, 0, BLACK);
            rect = create_rect((window_width-400)/2, 150, 100, 40);
            DrawRectangleRec(rect, GREEN);
            pos = (Vector2) {(window_width-400)/2, 150};
            DrawTextEx(get_font_from_config("consolas_32"), "YES", pos, 32, 0, BLACK);
            if (CheckCollisionPointRec(mouse_position, rect)) {
                set_cursor(CURSOR_INTERACT);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    game.current_screen = NO_SCREEN;
                }
            }
            rect = create_rect((window_width-400)/2+120, 150, 100, 40);
            DrawRectangleRec(rect, RED);
            pos = (Vector2) {(window_width-400)/2+120, 150};
            DrawTextEx(get_font_from_config("consolas_32"), "NO", pos, 32, 0, BLACK);
            if (CheckCollisionPointRec(mouse_position, rect)) {
                set_cursor(CURSOR_INTERACT);
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    game.blamed_character = NO_CHARACTER;
                    set_flag(BLAMING, false);
                }
            }
        }
        pos = (Vector2) { window_width-200, 10 };
        rect = create_rect(window_width-200, 10, 190, 40);
        if (get_flag(BLAMING))
            DrawRectangleRec(rect, GOLD);
        else
            DrawRectangleRec(rect, RED);
        DrawTextEx(get_font_from_config("consolas_32"), "BLAME", pos, 32, 0, BLACK);
        if (check_collision_and_valid(mouse_position, rect)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                game.blamed_character = NO_CHARACTER;
                toggle_flag(BLAMING);
            }
        }
    }
}

static void render_game_over(void)
{
    int window_width = GetScreenWidth();
    int window_height = GetScreenHeight();
    int size_x = 400;
    int size_y = 100;
    char buf[100];
    Vector2 pos = {(window_width-size_x)/2, (window_height-size_y)/2};
    if (game.blamed_character == SNAKE)
        sprintf(buf, "Snake was the killer");
    else
        sprintf(buf, "%s was not the killer", game.characters[game.blamed_character].display_name);
    DrawTextEx(get_font_from_config("consolas_32"), buf, pos, 32, 0, BLACK);
    pos.y += 100;
    Rectangle rect = create_rect(pos.x, pos.y, 100, 40);
    DrawRectangleRec(rect, GOLD);
    DrawTextEx(get_font_from_config("consolas_32"), "Back to menu", pos, 32, 0, BLACK);
    Vector2 mouse_position = GetMousePosition();
    if (CheckCollisionPointRec(mouse_position, rect)) {
        set_cursor(CURSOR_INTERACT);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            set_flag(IN_MENU, true);
        }
    }
}

static void render_game_objects(void)
{
    if (game.current_screen == NO_SCREEN) return;
    const char* background_texture_name = game.screens[game.current_screen].background_texture_name;
    Texture2D texture = get_texture_from_config(background_texture_name);
    draw_texture(texture, 0, 0, ctx.resolution.x, ctx.resolution.y);
    game.screens[game.current_screen].render();
    if (game.screens[game.current_screen].render_act[game.act] != NULL)
        game.screens[game.current_screen].render_act[game.act]();
}

static void key_callback(void)
{
    if (!get_flag(IN_MENU) && IsKeyPressed(KEY_ESCAPE))
        toggle_flag(MENU_OVERLAY);
    if (IsKeyPressed(KEY_Q))
        game.queried_item = ITEM_NONE;
}

void screen_transition(ScreenEnum screen)
{
    //timer_set(TIMER_SCREEN_TRANSITION, 1.5);
    //play_sound("walk");
    game.current_screen = screen;
    set_flag(IN_TRANSITION, true);
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

bool timer_isdone(TimerEnum timer)
{
    return game.timers[timer].done;
}

bool timer_isset(TimerEnum timer)
{
    return game.timers[timer].set;
}

static bool talked_to_all_characters(void)
{
    for (FlagEnum i = TALKED_TO_BEAR; i <= TALKED_TO_OWL; i++)
        if (!get_flag(i)) 
            return false;
    return true;
}

static void clear_talked_characters(void)
{
    for (FlagEnum i = TALKED_TO_BEAR; i <= TALKED_TO_OWL; i++)
        set_flag(i, false);
}

static void update_act1(void)
{
    if (!get_flag(TALKED_TO_ALL) && talked_to_all_characters()) {
        set_flag(TALKED_TO_ALL, true);
        create_dialogue(CROW, "I should talk to my father");
        clear_talked_characters();
    }

    if (!get_flag(EXAMINED_BEAR_NOTE) && game.queried_item == ITEM_BEAR_NOTE) {
        set_flag(EXAMINED_BEAR_NOTE, true);
    }
    if (!get_flag(BEAR_NOTE_RESPONSE) && get_flag(EXAMINED_BEAR_NOTE) && game.queried_item != ITEM_BEAR_NOTE) {
        set_flag(BEAR_NOTE_RESPONSE, true);
        create_dialogue(CROW, get_text_from_config("bear_note_crow_response"));
    }
}

static void update_act2(void)
{
    if (get_flag(TALKED_TO_FISH) && !get_flag(FINISHED_BATHROOM_CONVO) && get_flag(KNOCKED_ON_BATHROOM_DOOR) && !in_dialogue()) {
        set_flag(FINISHED_BATHROOM_CONVO, true);
        set_flag(TALKED_TO_FISH, false);
    }
    if (!get_flag(FINISHED_POST_BATHROOM_CONVO) && get_flag(TALKED_TO_BEAR) && !in_dialogue()) {
        set_flag(FINISHED_POST_BATHROOM_CONVO, true);
        create_dialogue(FISH, get_text_from_config("fish_hallway_1"));
        create_dialogue(BEAR, get_text_from_config("bear_hallway_2"));
    }
    if (!get_flag(FINISHED_HALLWAY_CONVO) && get_flag(FINISHED_POST_BATHROOM_CONVO) && !in_dialogue()) {
        set_flag(FINISHED_HALLWAY_CONVO, true);
        clear_talked_characters();
    }
    if (!get_flag(BEAR_ANNOUNCEMENT) 
        && get_flag(TALKED_TO_BEAR)
        && get_flag(TALKED_TO_FISH)
        && get_flag(TALKED_TO_SNAKE)
        && get_flag(TALKED_TO_OWL)
        && get_flag(TALKED_TO_CAT)) {

        create_dialogue(BEAR, get_text_from_config("bear_leave_dining"));
        set_flag(BEAR_ANNOUNCEMENT, true);
    }
    if (!get_flag(BEAR_WENT_TO_ROOM) && get_flag(BEAR_ANNOUNCEMENT) && !in_dialogue()) {
        set_flag(BEAR_WENT_TO_ROOM, true);
    }
    if (!get_flag(FISH_ANNOUNCEMENT) && get_flag(BEAR_WENT_TO_ROOM)) {
        create_dialogue(FISH, get_text_from_config("fish_leave_dining"));
        set_flag(FISH_ANNOUNCEMENT, true);
    }
    if (!get_flag(FISH_WENT_TO_ROOM) && get_flag(FISH_ANNOUNCEMENT) && !in_dialogue()) {
        set_flag(FISH_WENT_TO_ROOM, true);
        set_flag(TALKED_TO_BEAR, false);
        set_flag(TALKED_TO_FISH, false);
    }
    if (!get_flag(DOG_IN_MASTER_BEDROOM) && get_flag(FISH_WENT_TO_ROOM) && get_flag(TALKED_TO_FISH) && get_flag(TALKED_TO_BEAR) && !in_dialogue()) {
        set_flag(DOG_IN_MASTER_BEDROOM, true);
        create_dialogue(DOG, get_text_from_config("dog_shocked"));
    }
    if (get_flag(DOG_IN_MASTER_BEDROOM) && !in_dialogue()) {
        act_transition(ACT3);
        game.current_screen = SCREEN_LIVING_ROOM;
    }
}

static void update_act3(void)
{
    if (!get_flag(CONFRONTATION)) {
        set_flag(CONFRONTATION, true);
        create_dialogue(CROW, get_text_from_config("confrontation_1"));
    }
    if (!get_flag(DOG_GIVE_ATTIC_KEY) && get_flag(CONFRONTATION) && !in_dialogue()) {
        create_dialogue(DOG, get_text_from_config("dog_give_key"));
        give_item(ITEM_ATTIC_KEY);
        set_flag(DOG_GIVE_ATTIC_KEY, true);
    }
}

void game_update(float dt)
{
    set_flag(IN_TRANSITION, false);

    if (game.act_should_be != ACT_NONE && !in_dialogue()) {
        act_transition(game.act_should_be);
        game.act_should_be = ACT_NONE;
    }

    if (game.act == ACT1)
        update_act1();
    else if (game.act == ACT2)
        update_act2();
    else if (game.act == ACT3)
        update_act3();

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

    if (timer_isdone(TIMER_SCREEN_TRANSITION))
        timer_unset(TIMER_SCREEN_TRANSITION);

    key_callback();
}

void game_render(void)
{
    if (!get_flag(IN_MENU))
        render_game_objects();
}

void game_render_gui(void)
{
    if (get_flag(IN_MENU))
        render_menu_gui();
    else if (game.current_screen != NO_SCREEN)
        render_game_gui();
    else {
        render_game_over();
    }
}

void game_cleanup(void)
{
    DialogueNode* next;
    DialogueNode* cur;
    cur = game.dialogue_head;
    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }
}
