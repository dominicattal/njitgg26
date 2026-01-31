#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

#define DEFAULT_WINDOW_WIDTH (1920/2)
#define DEFAULT_WINDOW_HEIGHT (1080/2)

GlobalContext ctx;

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

void close_window_safely(void)
{
    ctx.window_exited = true;
}

static void state_cleanup(void)
{
    for (int i = 0; i < ctx.num_textures; i++)
        UnloadTexture(ctx.textures[i]);
    free(ctx.textures);
    json_object_destroy(ctx.texture_config);
}

static void window_key_callback(void)
{
    if (IsKeyPressed(KEY_F11)) {
        ToggleBorderlessWindowed();
    }
}

int main(void)
{
    ctx.window_exited = false;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "njitgg26");
    SetExitKey(KEY_NULL);

    InitAudioDevice();

    Music music = LoadMusicStream("assets/audio/rain.mp3");
    //PlayMusicStream(music);
    SetMusicVolume(music, 0.1f);

    state_init();
    game_init();

    SetTargetFPS(60);

    while (!ctx.window_exited)
    {
        window_key_callback();
        UpdateMusicStream(music);
        game_update(GetFrameTime());
        BeginDrawing();
            ClearBackground(RAYWHITE);
            game_render();
        EndDrawing();

        if (WindowShouldClose())
            ctx.window_exited = true;
    }

    game_cleanup();
    state_cleanup();

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
