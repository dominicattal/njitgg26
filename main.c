#include <stdio.h>
#include <assert.h>
#include "main.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

GlobalContext ctx;

bool showMessageBox = true;
void draw_start_menu(void)
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

void init_global_state(void)
{
    ctx.texture_config = json_read("config/textures.json");
    assert(ctx.texture_config);
}

Texture2D get_texture_from_config(const char* name)
{
    Image image;
    Texture2D texture;
    const char* path;
    JsonValue* value;
    value = json_object_get_value(ctx.texture_config, name);
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

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    init_global_state();

    InitWindow(screenWidth, screenHeight, "njitgg26");

    Texture2D texture = get_texture_from_config("placholder");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);
            DrawText("FUCK YESSS", 190, 200, 20, LIGHTGRAY);
            draw_start_menu();
        EndDrawing();
    }

    CloseWindow();
    UnloadTexture(texture);

    return 0;
}
