#include "game.h"
#include "main.h"
#include "raylib.h"
#include <stdlib.h>

static void render_item_foyer(void)
{
    Rectangle rect;
    int window_width = GetScreenWidth();
    int window_height = GetScreenHeight();
    rect = create_rect((window_width-400)/2, (window_height-400)/2, 400, 400);
    DrawRectangleRec(rect, (Color){255,100,255,100});
}

void item_init(void)
{
    game.items[ITEM_1].texture_name = "item1";
    game.items[ITEM_1].display_name = "item1";
    game.items[ITEM_2].texture_name = "item2";
    game.items[ITEM_2].display_name = "item2";
    game.items[ITEM_3].texture_name = "item3";
    game.items[ITEM_3].display_name = "item3";
    game.items[ITEM_FOYER].texture_name = "item_foyer";
    game.items[ITEM_FOYER].display_name = "Foy Cube";
    game.items[ITEM_FOYER].render_query = render_item_foyer;

    for (int i = 0; i < NUM_ITEMS; i++) {
        if (game.items[i].texture_name == NULL)
            TraceLog(LOG_FATAL, "missing texture name for item %d", i);
        if (game.items[i].display_name == NULL)
            TraceLog(LOG_FATAL, "missing display name for item %d", i);
        game.items[i].held = false;
    }
}
