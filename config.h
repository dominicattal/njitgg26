#ifndef CONFIG_H
#define CONFIG_H

#include <raylib.h>

Texture2D get_texture_from_config(const char* name);
char* get_text_from_config(const char* key);
Font get_font_from_config(const char* key);
Sound get_sound_from_config(const char* key);
void config_init(void);
void config_cleanup(void);

#endif
