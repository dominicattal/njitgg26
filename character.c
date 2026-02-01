#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "main.h"
#include "config.h"
#include "game.h"

#define CHARACTER_HITBOX_COLOR ((Color){0,255,255,100})

Rectangle render_character(CharacterEnum character, int x, int y)
{
    Rectangle rect = character_hitbox(character, x, y);
    Texture2D tex = get_texture_from_config(game.characters[character].texture_name);
    draw_texture_rect(tex, rect);
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
    Texture2D tex;
    game.characters[BEAR].display_name = "Bearon";
    game.characters[BEAR].portrait_texture_name = "bearon_portrait";
    game.characters[BEAR].texture_name = "bearon";
    tex = get_texture_from_config(game.characters[BEAR].texture_name);
    game.characters[BEAR].width = tex.width;
    game.characters[BEAR].height = tex.height;

    game.characters[CROW].display_name = "Crow";
    game.characters[CROW].portrait_texture_name = "player_portrait";
    game.characters[CROW].texture_name = "player";

    game.characters[FISH].display_name = "Fish";
    game.characters[FISH].portrait_texture_name = "fish_portrait";
    game.characters[FISH].texture_name = "fish";
    tex = get_texture_from_config(game.characters[FISH].texture_name);
    game.characters[FISH].width = tex.width;
    game.characters[FISH].height = tex.height;

    game.characters[PIG].display_name = "Pig";
    game.characters[PIG].portrait_texture_name = "pig_portrait";
    game.characters[PIG].texture_name = "pig";
    tex = get_texture_from_config(game.characters[PIG].texture_name);
    game.characters[PIG].width = tex.width;
    game.characters[PIG].height = tex.height;

    game.characters[DOG].display_name = "Dog";
    game.characters[DOG].portrait_texture_name = "dog_portrait";
    game.characters[DOG].texture_name = "dog";
    tex = get_texture_from_config(game.characters[DOG].texture_name);
    game.characters[DOG].width = tex.width;
    game.characters[DOG].height = tex.height;

    game.characters[SNAKE].display_name = "Snake";
    game.characters[SNAKE].portrait_texture_name = "snake_portrait";
    game.characters[SNAKE].texture_name = "snake";
    tex = get_texture_from_config(game.characters[SNAKE].texture_name);
    game.characters[SNAKE].width = tex.width;
    game.characters[SNAKE].height = tex.height;

    game.characters[CAT].display_name = "Cat";
    game.characters[CAT].portrait_texture_name = "cat_portrait";
    game.characters[CAT].texture_name = "cat";
    tex = get_texture_from_config(game.characters[CAT].texture_name);
    game.characters[CAT].width = tex.width;
    game.characters[CAT].height = tex.height;

    game.characters[OWL].display_name = "Owl";
    game.characters[OWL].portrait_texture_name = "owl_portrait";
    game.characters[OWL].texture_name = "owl";
    tex = get_texture_from_config(game.characters[OWL].texture_name);
    game.characters[OWL].width = tex.width;
    game.characters[OWL].height = tex.height;

    for (int i = 0; i < NUM_CHARACTERS; i++) {
        if (game.characters[i].texture_name == NULL)
            TraceLog(LOG_FATAL, "missing texture name for character %d", i);
        if (game.characters[i].portrait_texture_name == NULL)
            TraceLog(LOG_FATAL, "missing portrait name for character %d", i);
        if (game.characters[i].display_name == NULL)
            TraceLog(LOG_FATAL, "missing display name for character %d", i);
    }

}
