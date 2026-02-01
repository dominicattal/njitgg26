#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef void (*ScreenRenderFuncPtr)(void);
typedef void (*ScreenRenderGuiFuncPtr)(void);

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
    NUM_ITEMS
} ItemEnum;

typedef enum TimerEnum {
    TIMER_MENU_CAR,
    NUM_TIMERS
} TimerEnum;

typedef enum FlagEnum {
    FLAG_IN_MENU,
    FLAG_MENU_OVERLAY,

    // flags related to game state
    FLAG_PICKED_UP_FOYER_ITEM,

    NUM_FLAGS
} FlagEnum;

typedef struct Screen {
    ScreenRenderFuncPtr render;
    ScreenRenderGuiFuncPtr render_gui;
    const char* background_texture_name;
} Screen;

typedef struct Item {
    char* display_name;
    const char* texture_name;
    bool held;
} Item;

typedef struct Timer {
    float value;
    float max_value;
    bool active;
    bool set;
    bool done;
} Timer;

typedef struct GameState {
    Item items[NUM_ITEMS];
    Screen screens[NUM_SCREENS];
    Timer timers[NUM_TIMERS];
    bool flags[NUM_FLAGS];
    ScreenEnum current_screen;
} GameState;

extern GameState game;

void game_init(void);
void game_render_init(void);
void game_update(float dt);
void game_render(void);
void game_render_gui(void);
void game_cleanup(void);
void screen_transition(ScreenEnum screen);

void timer_set(TimerEnum timer, float max_value);
void timer_unset(TimerEnum timer);

void set_flag(FlagEnum flag, bool val);
bool toggle_flag(FlagEnum flag);
bool get_flag(FlagEnum flag);

void give_item(ItemEnum item);
void take_item(ItemEnum item);

#endif
