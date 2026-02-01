#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "main.h"

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

static Font load_font(const char* name, const JsonValue* value)
{
    Font font;
    int font_size;
    JsonArray* array;
    const char* path;
    if (value == NULL) {
        TraceLog(LOG_WARNING, "%s not found in font config, using default font", name);
        goto return_placeholder;
    }
    if (json_value_get_type(value) != JTYPE_ARRAY) {
        TraceLog(LOG_WARNING, "%s must be an array like [path, font_size], defaulting to placeholder", name);
        goto return_placeholder;
    }
    array = json_value_get_array(value);
    if (json_array_length(array) != 2) {
        TraceLog(LOG_WARNING, "%s must be an array like [path, font_size], defaulting to placeholder", name);
        goto return_placeholder;
    }
    value = json_array_get(array, 0);
    if (json_value_get_type(value) != JTYPE_STRING) {
        TraceLog(LOG_WARNING, "%s must be an array like [path, font_size], defaulting to placeholder", name);
        goto return_placeholder;
    }
    path = json_value_get_string(value);
    value = json_array_get(array, 1);
    if (json_value_get_type(value) != JTYPE_INT) {
        TraceLog(LOG_WARNING, "%s must be an array like [path, font_size], defaulting to placeholder", name);
        goto return_placeholder;
    }
    font_size = json_value_get_int(value);
    font = LoadFontEx(path, font_size, NULL, 250);
    if (!IsFontValid(font)) {
        TraceLog(LOG_WARNING, "Could not load font, default to placeholder", name);
        goto return_placeholder;
    }
    return font;

return_placeholder:
    return GetFontDefault();
}

static Sound load_sound(const char* name, const JsonValue* value)
{
    Sound sound;
    const char* path;
    if (value == NULL) {
        TraceLog(LOG_WARNING, "%s not found in audio config", name);
        goto return_placeholder;
    }
    if (json_value_get_type(value) != JTYPE_STRING) {
        TraceLog(LOG_WARNING, "%s wrong type", name);
        goto return_placeholder;
    }
    path = json_value_get_string(value);
    sound = LoadSound(path);
    if (!IsSoundValid(sound)) {
        TraceLog(LOG_WARNING, "failed to create sound");
        goto return_placeholder;
    }
    return sound;

return_placeholder:
    value = json_object_get_value(ctx.sound_config, "placeholder");
    assert(value);
    assert(json_value_get_type(value) == JTYPE_STRING);
    path = json_value_get_string(value);
    assert(path);
    sound = LoadSound(path);
    assert(IsSoundValid(sound));
    return sound;
}

void config_init(void)
{
    JsonMember* member;
    JsonIterator* it;
    JsonValue* value;
    const char* key;
    int i;

    ctx.texture_config = json_read("config/textures.json");
    if (ctx.texture_config == NULL)
        TraceLog(LOG_FATAL, "could not read texture config file, probably because of a syntax error");

    ctx.text_config = json_read("config/text.json");
    if (ctx.text_config == NULL)
        TraceLog(LOG_FATAL, "could not read text config file, probably because of a syntax error");

    ctx.font_config = json_read("config/fonts.json");
    if (ctx.font_config == NULL)
        TraceLog(LOG_FATAL, "could not read font config file, probably because of a syntax error");

    ctx.sound_config = json_read("config/sounds.json");
    if (ctx.sound_config == NULL)
        TraceLog(LOG_FATAL, "could not read sound config file, probably because of a syntax error");


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

    ctx.num_fonts = json_object_length(ctx.font_config);
    ctx.font_names = malloc(ctx.num_fonts * sizeof(Font));
    ctx.fonts = malloc(ctx.num_fonts * sizeof(Font));
    it = json_iterator_create(ctx.font_config);
    for (i = 0; i < ctx.num_fonts; i++) {
        member = json_iterator_get(it);
        key = json_member_get_key(member);
        value = json_member_get_value(member);
        ctx.font_names[i] = key;
        ctx.fonts[i] = load_font(key, value);
        json_iterator_increment(it); 
    }
    json_iterator_destroy(it);

    ctx.num_sounds = json_object_length(ctx.sound_config);
    ctx.sound_names = malloc(ctx.num_sounds * sizeof(const char*));
    ctx.sounds = malloc(ctx.num_sounds * sizeof(Sound));
    it = json_iterator_create(ctx.sound_config);
    for (i = 0; i < ctx.num_sounds; i++) {
        member = json_iterator_get(it);
        key = json_member_get_key(member);
        value = json_member_get_value(member);
        ctx.sound_names[i] = key;
        ctx.sounds[i] = load_sound(key, value);
        json_iterator_increment(it); 
    }
    json_iterator_destroy(it);

}

Texture2D get_texture_from_config(const char* name)
{
    int l, r, m, a;
    l = 0;
    r = ctx.num_textures-1;
    assert(name != NULL);
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
    TraceLog(LOG_WARNING, "Could not get texture id for %s", name);
    return get_texture_from_config("placeholder");
}

Font get_font_from_config(const char* name)
{
    int l, r, m, a;
    l = 0;
    r = ctx.num_fonts-1;
    assert(name != NULL);
    while (l <= r) {
        m = l + (r - l) / 2;
        a = strcmp(name, ctx.font_names[m]);
        if (a > 0)
            l = m + 1;
        else if (a < 0)
            r = m - 1;
        else
            return ctx.fonts[m];
    }
    TraceLog(LOG_WARNING, "Could not get font id for %s, returing default font", name);
    return GetFontDefault();
}

char* get_text_from_config(const char* key)
{
    JsonValue* value = json_object_get_value(ctx.text_config, key);
    if (value == NULL) {
        TraceLog(LOG_WARNING, "Missing text %s, using default", key);
        goto return_placeholder;
    }
    if (json_value_get_type(value) != JTYPE_STRING) {
        TraceLog(LOG_WARNING, "Incorrect json value for text %s, using default", key);
        goto return_placeholder;
    }
    return json_value_get_string(value);
return_placeholder:
    value = json_object_get_value(ctx.text_config, "placeholder");
    assert(value);
    assert(json_value_get_type(value) == JTYPE_STRING);
    return json_value_get_string(value);
}

Sound get_sound_from_config(const char* name)
{
    int l, r, m, a;
    l = 0;
    r = ctx.num_sounds-1;
    assert(name != NULL);
    while (l <= r) {
        m = l + (r - l) / 2;
        a = strcmp(name, ctx.sound_names[m]);
        if (a > 0)
            l = m + 1;
        else if (a < 0)
            r = m - 1;
        else
            return ctx.sounds[m];
    }
    TraceLog(LOG_WARNING, "Could not get sound id for %s, returing default sound", name);
    return get_sound_from_config("placeholder");
}

void config_cleanup(void)
{
    for (int i = 0; i < ctx.num_textures; i++)
        UnloadTexture(ctx.textures[i]);
    for (int i = 0; i < ctx.num_fonts; i++)
        UnloadFont(ctx.fonts[i]);
    //for (int i = 0; i < ctx.num_sounds; i++)
    //    UnloadSound(ctx.sounds[i]);
    free(ctx.texture_names);
    free(ctx.textures);
    free(ctx.font_names);
    free(ctx.fonts);
    //free(ctx.sound_names);
    //free(ctx.sounds);
    json_object_destroy(ctx.texture_config);
    json_object_destroy(ctx.text_config);
    json_object_destroy(ctx.font_config);
    json_object_destroy(ctx.sound_config);
}
