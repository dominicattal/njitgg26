#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

#define RESOLUTION_X 1920
#define RESOLUTION_Y 1080
#define DEFAULT_WINDOW_WIDTH (1920/2)
#define DEFAULT_WINDOW_HEIGHT (1080/2)

GlobalContext ctx;

static Texture2D load_texture(const char* name, JsonValue* value)
{
    Image image;
    Texture2D texture;
    const char* path;
    if (value == NULL) {
        TraceLog(LOG_WARNING, "%s not found in texture config, defaulting to placeholder", name);
        goto return_placeholder;
    }
    if (json_value_get_type(value) != JTYPE_STRING) {
        TraceLog(LOG_WARNING, "%s wrong type, defaulting to placeholder", name);
        goto return_placeholder;
    }
    path = json_value_get_string(value);
    image = LoadImage(path);
    if (!IsImageValid(image)) {
        TraceLog(LOG_WARNING, "path for texture %s (%s) not found in texture config, defaulting to placeholder", name, path);
        goto return_placeholder;
    }
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    if (!IsTextureValid(texture)) {
        TraceLog(LOG_WARNING, "failed to create texture, returning placeholder");
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
        TraceLog(LOG_FATAL, "could not read texture config file, probably because of a syntax error");

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

    if (json_object_get_value(ctx.texture_config, "placeholder") == NULL)
        TraceLog(LOG_FATAL, "couldn't get placeholder texture");
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
    TraceLog(LOG_WARNING, "Could not get id for %s", name);
    return get_texture_from_config("placeholder");
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

Vector2 get_scaled_mouse_position(void)
{
    Vector2 mouse_point = GetMousePosition();
    mouse_point.x *= (float)ctx.resolution.x / GetScreenWidth();
    mouse_point.y *= (float)ctx.resolution.y / GetScreenHeight();
    return mouse_point;
}

Rectangle create_rect(float x, float y, float width, float height)
{
    if (x < 0) x = ctx.resolution.x - x - width + 1;
    if (y < 0) y = ctx.resolution.y - y - height + 1;
    return (Rectangle){x,y,width,height};
}

Rectangle create_rect2(float x1, float y1, float x2, float y2)
{
    return (Rectangle){x1,y1,x2-x1+1,y2-y1+1};
}

float lerp(float from, float to, float t, float max_t)
{
    return (to-from) * t / max_t + from;
}

void set_cursor(CursorEnum cursor)
{
    ctx.current_cursor = cursor;
}

int main(void)
{
    RenderTexture2D framebuffer;

    ctx.resolution.x = RESOLUTION_X;
    ctx.resolution.y = RESOLUTION_Y;
    ctx.window_exited = false;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "njitgg26");
    SetExitKey(KEY_NULL);
    HideCursor();

    InitAudioDevice();

    Music music = LoadMusicStream("assets/audio/rain.mp3");
    //PlayMusicStream(music);
    SetMusicVolume(music, 0.1f);

    framebuffer = LoadRenderTexture(RESOLUTION_X, RESOLUTION_Y);

    state_init();
    game_init();

    SetTargetFPS(60);

    ctx.cursor_textures[CURSOR_NORMAL] = get_texture_from_config("cursor_normal");
    ctx.cursor_textures[CURSOR_INTERACT] = get_texture_from_config("cursor_interact");

    while (!ctx.window_exited)
    {
        ctx.current_cursor = CURSOR_NORMAL;
        window_key_callback();
        UpdateMusicStream(music);
        game_update(GetFrameTime());
        BeginTextureMode(framebuffer);
            ClearBackground(RAYWHITE);
            game_render();
        EndTextureMode();
        BeginDrawing();
            ClearBackground(BLUE);

            Rectangle src, dst;
            src = (Rectangle) { 0, 0, framebuffer.texture.width, -framebuffer.texture.height };
            dst = (Rectangle) { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
            DrawTexturePro(framebuffer.texture, src, dst, (Vector2) {0,0}, 0, (Color){255,255,255,255});

            game_render_gui();

            DrawTextureV(ctx.cursor_textures[ctx.current_cursor], GetMousePosition(), (Color){255,255,255,255});

        EndDrawing();

        if (WindowShouldClose())
            ctx.window_exited = true;
    }

    game_cleanup();
    state_cleanup();

    UnloadRenderTexture(framebuffer);

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
