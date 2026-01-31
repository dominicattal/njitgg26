#include <stdio.h>
#include <assert.h>
#include "main.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

GlobalContext ctx;

bool showMessageBox = true;
static void draw_start_menu(void)
{
    if (GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Show Message"))
        showMessageBox = true;
    if (showMessageBox) 
    {
        int result = GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
            "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

        if (result >= 0) showMessageBox = false;
    }
}

static Texture2D load_texture(const char* name, JsonValue* value)
{
    Image image;
    Texture2D texture;
    const char* path;
    if (value == NULL) {
        log_write(WARNING, "%s not found in texture config, defaulting to placeholder", name);
        goto return_placeholder;
    }
    if (json_value_get_type(value) != JTYPE_STRING) {
        log_write(WARNING, "%s wrong type, defaulting to placeholder", name);
        goto return_placeholder;
    }
    path = json_value_get_string(value);
    image = LoadImage(path);
    if (!IsImageValid(image)) {
        log_write(WARNING, "path for texture %s (%s) not found in texture config, defaulting to placeholder", name, path);
        goto return_placeholder;
    }
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    if (!IsTextureValid(texture)) {
        log_write(WARNING, "failed to create texture, returning placeholder");
        goto return_placeholder;
    }
    return texture;

return_placeholder:
    value = json_object_get_value(ctx.texture_config, "placeholder");
    assert(value);
    assert(json_value_get_type(value) == JTYPE_STRING);
    path = json_value_get_string(value);
    assert(path);
    image = LoadImage(path);
    assert(IsImageValid(image));
    texture = LoadTextureFromImage(image);
    assert(IsTextureValid(texture));
    UnloadImage(image);
    return texture;
}

static void state_init(void)
{
    JsonMember* member;
    JsonIterator* it;
    JsonValue* value;
    const char* key;
    int i;

    ctx.texture_config = json_read("config/textures.json");
    if (ctx.texture_config == NULL)
        log_write(FATAL, "could not read texture config file, probably because of a syntax error");

    ctx.num_textures = json_object_length(ctx.texture_config);
    ctx.texture_names = malloc(ctx.num_textures * sizeof(const char*));
    ctx.textures = malloc(ctx.num_textures * sizeof(Texture2D));
    it = json_iterator_create(ctx.texture_config);
    for (i = 0; i < ctx.num_textures; i++) {
        member = json_iterator_get(it);
        key = json_member_get_key(member);
        value = json_member_get_value(member);
        ctx.texture_names[i] = key;
        ctx.textures[i] = load_texture(key, value);
        json_iterator_increment(it); 
    }
    json_iterator_destroy(it);
}

static void state_cleanup(void)
{
    free(ctx.textures);
    json_object_destroy(ctx.texture_config);
}

Texture2D get_texture_from_config(const char* name)
{
    int l, r, m, a;
    l = 0;
    r = ctx.num_textures-1;
    while (l <= r) {
        m = l + (r - l) / 2;
        a = strcmp(name, ctx.texture_names[m]);
        if (a > 0)
            l = m + 1;
        else if (a < 0)
            r = m - 1;
        else
            return ctx.textures[m];
    }
    log_write(WARNING, "Could not get id for %s", name);
    return ctx.textures[0];
}

int main(void)
{
    ctx.window_width = 800;
    ctx.window_height = 450;
    InitWindow(ctx.window_width, ctx.window_height, "njitgg26");
    state_init();

    game_init();

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            game_render();
            DrawText("FUCK YESSS", 190, 200, 20, LIGHTGRAY);
            draw_start_menu();
        EndDrawing();
    }

    CloseWindow();

    game_cleanup();
    state_cleanup();

    return 0;
}
