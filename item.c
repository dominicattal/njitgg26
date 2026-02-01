#include "game.h"
#include "main.h"
#include "raylib.h"
#include "gui.h"
#include "config.h"
#include <stdlib.h>

static void render_item_foyer(void)
{
    Rectangle rect;
    int window_width = GetScreenWidth();
    int window_height = GetScreenHeight();
    rect = create_rect((window_width-400)/2, (window_height-400)/2, 400, 400);
    DrawRectangleRec(rect, (Color){255,100,255,100});
}

static void render_item_bear_note(void)
{
    Rectangle rect;
    int window_width = GetScreenWidth();
    int window_height = GetScreenHeight();
    rect = create_rect((window_width-400)/2, (window_height-400)/2, 400, 400);
    DrawRectangleRec(rect, BLACK);
    DrawTextBoxed(get_font_from_config("consolas_16"), get_text_from_config("bear_note_content"), rect, 16, 0, true, WHITE);
}

static void render_item_book(void)
{
    Rectangle rect;
    int window_width = GetScreenWidth();
    int window_height = GetScreenHeight();
    rect = create_rect((window_width-400)/2, (window_height-400)/2, 400, 400);
    DrawRectangleRec(rect, BLACK);
    DrawTextBoxed(get_font_from_config("consolas_16"), get_text_from_config("book_content"), rect, 16, 0, true, WHITE);
}

void item_init(void)
{
    game.items[ITEM_FOYER].texture_name = "item_foyer";
    game.items[ITEM_FOYER].display_name = "Foy Cube";
    game.items[ITEM_FOYER].render_query = render_item_foyer;

    game.items[ITEM_BEAR_NOTE].texture_name = "item_bear_note";
    game.items[ITEM_BEAR_NOTE].display_name = "Note";
    game.items[ITEM_BEAR_NOTE].render_query = render_item_bear_note;

    game.items[ITEM_BEAR_THING].texture_name = "item_bear_thing";
    game.items[ITEM_BEAR_THING].display_name = "Bear Thing";

    game.items[ITEM_KNIFE].texture_name = "item_knife";
    game.items[ITEM_KNIFE].display_name = "Knife";
    //game.items[ITEM_KNIFE].render_query = render_item_bear_note;

    game.items[ITEM_BOOK].texture_name = "item_book";
    game.items[ITEM_BOOK].display_name = "Book";
    game.items[ITEM_BOOK].render_query = render_item_book;

    game.items[ITEM_DEED].texture_name = "item_deed";
    game.items[ITEM_DEED].display_name = "Deed";
    //game.items[ITEM_DEED].render_query = render_item_bear_note;

    game.items[ITEM_ATTIC_KEY].texture_name = "item_attic_key";
    game.items[ITEM_ATTIC_KEY].display_name = "Attic Key";
    //game.items[ITEM_ATTIC_KEY].render_query = render_item_bear_note;

    game.items[ITEM_CHEST_KEY].texture_name = "item_chest_key";
    game.items[ITEM_CHEST_KEY].display_name = "Chest Key";
    //game.items[ITEM_ATTIC_KEY].render_query = render_item_bear_note;

    for (int i = 0; i < NUM_ITEMS; i++) {
        if (game.items[i].texture_name == NULL)
            TraceLog(LOG_FATAL, "missing texture name for item %d", i);
        if (game.items[i].display_name == NULL)
            TraceLog(LOG_FATAL, "missing display name for item %d", i);
        game.items[i].held = false;
    }
}
