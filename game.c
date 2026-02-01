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

void game_init(void)
{
    set_flag(FLAG_IN_MENU, true);
    game.current_screen = SCREEN_FOYER;
    game.dialogue_head = NULL;
    game.dialogue_tail = NULL;

    screen_init();
    item_init();
    character_init();

    // setting game state for testing
    set_flag(FLAG_IN_MENU, false);
    game.selected_item = ITEM_NONE;
    game.queried_item = ITEM_NONE;
}

static void draw_texture(Texture2D tex, float x, float y, float w, float h)
{
    Rectangle src, dst;
    src = (Rectangle) { 0, 0, tex.width, tex.height };
    dst = (Rectangle) { x, y, w, h };
    DrawTexturePro(tex, src, dst, (Vector2) {0,0}, 0, (Color){255,255,255,255});
}

static void draw_texture_rect(Texture2D tex, Rectangle rect)
{
    Rectangle src;
    src = (Rectangle) { 0, 0, tex.width, tex.height };
    DrawTexturePro(tex, src, rect, (Vector2) {0,0}, 0, (Color){255,255,255,255});
}

static void start_game(void)
{
    for (int flag = 0; flag < NUM_FLAGS; flag++)
        set_flag(flag, false);

    game.selected_item = ITEM_NONE;
    game.queried_item = ITEM_NONE;
    game.act = ACT1;
}

static void end_game(void)
{
    set_flag(FLAG_IN_MENU, true);
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
        set_flag(FLAG_MENU_OVERLAY, false);
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

    for (i = 0; i < NUM_ITEMS; i++)
        if (game.items[i].held)
            num_held_items++;

    rect = create_rect(offset_x-10, offset_y-10, (num_held_items-1)*75+dim+20, dim+20);
    DrawRectangleRec(rect, (Color){40,40,40,100});
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
        if (i == game.queried_item)
            DrawRectangleRec(rect, (Color){100,255,255,120});

        draw_texture_rect(tex, rect);
        cur_offset_x += 75;

        if (i == game.queried_item) {
            item->render_query();
        }
    }

    if (game.screens[game.current_screen].render_gui != NULL)
        game.screens[game.current_screen].render_gui();
    if (get_flag(FLAG_MENU_OVERLAY))
        render_menu_overlay();

    if (item_info != NULL) {
        rect = create_rect(offset_x-10, offset_y+dim+10, 300, 30);
        DrawRectangleRec(rect, BLUE);
        DrawTextBoxed(get_font_from_config("consolas_16"), item_display_name, rect, 16, 0, true, WHITE);
        rect = create_rect(offset_x-10, offset_y+dim+40, 300, 100);
        DrawRectangleRec(rect, PURPLE);
        DrawTextBoxed(get_font_from_config("consolas_16"), item_info, rect, 16, 0, true, WHITE);
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
        mouse_position = GetMousePosition();
        if (CheckCollisionPointRec(mouse_position, rect)) {
            set_cursor(CURSOR_INTERACT);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                advance_dialogue();
        }
    }
}

static void render_game_objects(void)
{
    const char* background_texture_name = game.screens[game.current_screen].background_texture_name;
    Texture2D texture = get_texture_from_config(background_texture_name);
    draw_texture(texture, 0, 0, ctx.resolution.x, ctx.resolution.y);
    game.screens[game.current_screen].render();
    if (game.screens[game.current_screen].render_act[game.act] != NULL)
        game.screens[game.current_screen].render_act[game.act]();
}

static void key_callback(void)
{
    if (!get_flag(FLAG_IN_MENU) && IsKeyPressed(KEY_ESCAPE))
        toggle_flag(FLAG_MENU_OVERLAY);
    if (IsKeyPressed(KEY_Q))
        game.queried_item = ITEM_NONE;
}

void screen_transition(ScreenEnum screen)
{
    //timer_set(TIMER_SCREEN_TRANSITION, 1.5);
    //play_sound("walk");
    game.current_screen = screen;
    set_flag(FLAG_IN_TRANSITION, true);
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

void game_update(float dt)
{
    set_flag(FLAG_IN_TRANSITION, false);

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
    if (!get_flag(FLAG_IN_MENU))
        render_game_objects();
}

void game_render_gui(void)
{
    if (get_flag(FLAG_IN_MENU))
        render_menu_gui();
    else
        render_game_gui();
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
