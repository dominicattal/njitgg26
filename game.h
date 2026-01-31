#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef void (*ScreenRenderFuncPtr)(void);

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

typedef struct Screen Screen;
typedef struct Screen {
    ScreenRenderFuncPtr render;
    const char* background_texture_name;
} Screen;

typedef struct GameState {
    ScreenEnum current_screen;
    Screen* screens;
    bool in_menu;
} GameState;

extern GameState game;

void game_init(void);
void game_render(void);
void game_cleanup(void);

#endif
