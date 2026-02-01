#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "raylib.h"

typedef void (*ScreenRenderFuncPtr)(void);
typedef void (*ScreenRenderGuiFuncPtr)(void);
typedef void (*ItemRenderQueryFuncPtr)(void);
typedef void (*CharacterRenderFuncPtr)(int x, int y);

typedef enum ScreenEnum {
    SCREEN_LIVING_ROOM,
    SCREEN_FOYER,
    SCREEN_DINING_ROOM,
    SCREEN_KITCHEN,
    SCREEN_BASEMENT,
    SCREEN_MASTER_BEDROOM,
    SCREEN_HALLWAY,
    SCREEN_GUEST_BEDROOM,
    SCREEN_BATHROOM,
    SCREEN_ATTIC,
    NUM_SCREENS,
    NO_SCREEN
} ScreenEnum;

typedef enum ItemEnum {
    ITEM_FOYER,
    ITEM_BEAR_NOTE,
    ITEM_BEAR_THING,
    ITEM_KNIFE,
    ITEM_BOOK,
    ITEM_DEED,
    ITEM_ATTIC_KEY,
    NUM_ITEMS,
    ITEM_NONE
} ItemEnum;

typedef enum TimerEnum {
    TIMER_MENU_CAR,
    TIMER_SCREEN_TRANSITION,
    NUM_TIMERS
} TimerEnum;

typedef enum FlagEnum {
    IN_MENU,
    MENU_OVERLAY,
    IN_TRANSITION,

    // flags related to game state
    PICKED_UP_FOYER_ITEM,
    PICKED_UP_KNIFE,
    PICKED_UP_BOOK,
    PICKED_UP_DEED,
    OPENED_ATTIC_DOOR,
    TALKED_TO_BEAR,
    TALKED_TO_FISH,
    TALKED_TO_PIG,
    TALKED_TO_DOG,
    TALKED_TO_SNAKE,
    TALKED_TO_CAT,
    TALKED_TO_OWL,
    TALKED_TO_ALL,

    // act1 flags
    EXAMINED_BEAR_NOTE,
    BEAR_NOTE_RESPONSE,
    PICKED_UP_BEAR_NOTE,
    PICKED_UP_BEAR_THING,

    // act2 flags
    KNOCKED_ON_BATHROOM_DOOR,
    FINISHED_BATHROOM_CONVO,
    FINISHED_POST_BATHROOM_CONVO,
    FINISHED_HALLWAY_CONVO,
    BEAR_ANNOUNCEMENT,
    BEAR_WENT_TO_ROOM,
    FISH_ANNOUNCEMENT,
    FISH_WENT_TO_ROOM,
    DOG_IN_MASTER_BEDROOM,

    // act3 flags
    CONFRONTATION,
    DOG_GIVE_ATTIC_KEY,
    BLAMING,
    BLAME_CONFIRM,
    BLAMED,

    NUM_FLAGS
} FlagEnum;

typedef enum CharacterEnum {
    CROW,
    BEAR,
    FISH,
    PIG,
    DOG,
    SNAKE,
    CAT,
    OWL,
    NUM_CHARACTERS,
    NO_CHARACTER
} CharacterEnum;

typedef enum ActEnum {
    ACT1,
    ACT2,
    ACT3,
    NUM_ACTS,
    ACT_NONE
} ActEnum;

typedef struct Screen {
    ScreenRenderFuncPtr render;
    ScreenRenderGuiFuncPtr render_gui;
    ScreenRenderFuncPtr render_act[NUM_ACTS];
    char* background_texture_name;
} Screen;

typedef struct Item {
    ItemRenderQueryFuncPtr render_query;
    char* display_name;
    char* texture_name;
    bool held;
} Item;

typedef struct Timer {
    float value;
    float max_value;
    bool active;
    bool set;
    bool done;
} Timer;

typedef struct Character {
    char* display_name;
    char* portrait_texture_name;
    char* texture_name;
    int width, height;
} Character;

typedef struct DialogueNode DialogueNode;
typedef struct DialogueNode {
    DialogueNode* next;
    CharacterEnum character;
    const char* dialogue;
} DialogueNode;

typedef struct GameState {
    Item items[NUM_ITEMS];
    Screen screens[NUM_SCREENS];
    Timer timers[NUM_TIMERS];
    bool flags[NUM_FLAGS];
    Character characters[NUM_CHARACTERS];
    CharacterEnum blamed_character;
    ScreenEnum current_screen;
    ItemEnum selected_item;
    ItemEnum queried_item;
    DialogueNode* dialogue_head;
    DialogueNode* dialogue_tail;
    ActEnum act_should_be;
    ActEnum act;
} GameState;

extern GameState game;

void game_init(void);
void game_update(float dt);
void game_render(void);
void game_render_gui(void);
void game_cleanup(void);

void screen_init(void);
void screen_transition(ScreenEnum screen);

void act_transition(ActEnum act);

void item_init(void);

void character_init(void);

void timer_set(TimerEnum timer, float max_value);
void timer_unset(TimerEnum timer);
bool timer_isdone(TimerEnum timer);
bool timer_isset(TimerEnum timer);

void set_flag(FlagEnum flag, bool val);
bool toggle_flag(FlagEnum flag);
bool get_flag(FlagEnum flag);

void give_item(ItemEnum item);
void take_item(ItemEnum item);
bool has_item(ItemEnum item);
bool selected_item(ItemEnum item);

char* character_display_name(CharacterEnum character);
// returns character hitbox
Rectangle render_character(CharacterEnum character, int x, int y);
Rectangle character_hitbox(CharacterEnum character, int x, int y);

void create_dialogue(CharacterEnum character, const char* dialogue);
void advance_dialogue(void);
bool in_dialogue(void);

void draw_texture(Texture2D tex, float x, float y, float w, float h);
void draw_texture_rect(Texture2D tex, Rectangle rect);
Rectangle draw_texture_def(Texture2D tex, float x, float y);
Rectangle hitbox_from_hitbox(Rectangle hitbox, float x1, float y1, float x2, float y2);

bool check_collision_and_valid(Vector2 pos, Rectangle rect);

#endif
