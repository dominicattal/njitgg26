#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "main.h"
#include "game.h"

#define CHARACTER_HITBOX_COLOR ((Color){0,255,255,100})

Rectangle render_character(CharacterEnum character, int x, int y)
{
    Rectangle rect = character_hitbox(character, x, y);
    DrawRectangleRec(rect, CHARACTER_HITBOX_COLOR);
    return rect;
}

Rectangle character_hitbox(CharacterEnum character, int x, int y)
{
    Character* c = &game.characters[character];
    Rectangle rect = (Rectangle) {
        .x = x - c->width/2,
        .y = y - c->height,
        .width = c->width,
        .height = c->height
    };
    return rect;
}

void character_init(void)
{
    game.characters[BEAR].display_name = "Bearon";
    game.characters[BEAR].portrait_texture_name = "bearon_portrait";
    game.characters[BEAR].texture_name = "bearon";
    game.characters[BEAR].width = 220;
    game.characters[BEAR].height = 300;

    game.characters[CROW].display_name = "Crow";
    game.characters[CROW].portrait_texture_name = "player_portrait";
    game.characters[CROW].texture_name = "player";

    game.characters[FISH].display_name = "Fish";
    game.characters[FISH].portrait_texture_name = "fish_portrait";
    game.characters[FISH].texture_name = "fish";
    game.characters[FISH].width = 220;
    game.characters[FISH].height = 300;

    game.characters[PIG].display_name = "Pig";
    game.characters[PIG].portrait_texture_name = "pig_portrait";
    game.characters[PIG].texture_name = "pig";
    game.characters[PIG].width = 220;
    game.characters[PIG].height = 300;

    game.characters[DOG].display_name = "Dog";
    game.characters[DOG].portrait_texture_name = "dog_portrait";
    game.characters[DOG].texture_name = "dog";

    game.characters[SNAKE].display_name = "Snake";
    game.characters[SNAKE].portrait_texture_name = "snake_portrait";
    game.characters[SNAKE].texture_name = "snake";

    game.characters[CAT].display_name = "Cat";
    game.characters[CAT].portrait_texture_name = "cat_portrait";
    game.characters[CAT].texture_name = "cat";

    game.characters[OWL].display_name = "Owl";
    game.characters[OWL].portrait_texture_name = "owl_portrait";
    game.characters[OWL].texture_name = "owl";

    for (int i = 0; i < NUM_CHARACTERS; i++) {
        if (game.characters[i].texture_name == NULL)
            TraceLog(LOG_FATAL, "missing texture name for character %d", i);
        if (game.characters[i].portrait_texture_name == NULL)
            TraceLog(LOG_FATAL, "missing portrait name for character %d", i);
        if (game.characters[i].display_name == NULL)
            TraceLog(LOG_FATAL, "missing display name for character %d", i);
    }

}
