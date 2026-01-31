#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef enum ScreenEnum {
    SCREEN_1,
    SCREEN_2,
    SCREEN_3
} ScreenEnum;

typedef struct Screen Screen;
typedef struct Screen {
    ScreenEnum* connected_screens;
    int num_connected_screens;
} Screen;

typedef struct GameState {
    ScreenEnum current_screen;
    bool event1;
    bool event2;
} GameState;

void game_init(void);
void game_render(void);
void game_cleanup(void);

#endif
