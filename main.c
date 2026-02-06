#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "config.h"

#define RESOLUTION_X 1920
#define RESOLUTION_Y 1080
#define DEFAULT_WINDOW_WIDTH (1920/2)
#define DEFAULT_WINDOW_HEIGHT (1080/2)

GlobalContext ctx;

void close_window_safely(void)
{
    ctx.window_exited = true;
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

void play_sound(const char* name)
{
    Sound sound = get_sound_from_config(name);
    if (IsSoundPlaying(sound))
        StopSound(sound);
    PlaySound(sound);
}

int main(void)
{
    RenderTexture2D framebuffer;

    ctx.resolution.x = RESOLUTION_X;
    ctx.resolution.y = RESOLUTION_Y;
    ctx.window_exited = false;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "njitgg26");
    //ToggleBorderlessWindowed();
    SetExitKey(KEY_NULL);
    HideCursor();

    InitAudioDevice();

    // get rid of this
    ctx.font = LoadFontEx("assets/fonts/consola.ttf", 32, NULL, 250);
    if (!IsFontValid(ctx.font))
        TraceLog(LOG_FATAL, "couldn't load font");

    Music music = LoadMusicStream("assets/audio/rain.mp3");
    //PlayMusicStream(music);
    SetMusicVolume(music, 0.1f);

    framebuffer = LoadRenderTexture(RESOLUTION_X, RESOLUTION_Y);

    config_init();
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
    config_cleanup();

    UnloadRenderTexture(framebuffer);
    UnloadFont(ctx.font);

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
