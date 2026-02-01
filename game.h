#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef void (*ScreenRenderFuncPtr)(void);
typedef void (*ScreenRenderGuiFuncPtr)(void);
typedef void (*ItemRenderQueryFuncPtr)(void);

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
    NUM_SCREENS
} ScreenEnum;

typedef enum ItemEnum {
    ITEM_FOYER,
    ITEM_1,
    ITEM_2,
    ITEM_3,
    NUM_ITEMS,
    ITEM_NONE
} ItemEnum;

typedef enum TimerEnum {
    TIMER_MENU_CAR,
    TIMER_SCREEN_TRANSITION,
    NUM_TIMERS
} TimerEnum;

typedef enum FlagEnum {
    FLAG_IN_MENU,
    FLAG_MENU_OVERLAY,

    // flags related to game state
    FLAG_PICKED_UP_FOYER_ITEM,
    FLAG_OPENED_ATTIC_DOOR,

    NUM_FLAGS
} FlagEnum;

typedef enum CharacterEnum {
    PLAYER,
    BEARON,
    NUM_CHARACTERS,
    NO_CHARACTER
} CharacterEnum;

typedef enum ActEnum {
    ACT1,
    ACT2,
    ACT3,
    NUM_ACTS
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
    ScreenEnum current_screen;
    ItemEnum selected_item;
    ItemEnum queried_item;
    DialogueNode* dialogue_head;
    DialogueNode* dialogue_tail;
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

void item_init(void);

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

void create_dialogue(CharacterEnum character, const char* dialogue);
void advance_dialogue(void);
bool in_dialogue(void);

#endif
