#ifndef RAYLIB_CONFIG_H
#define RAYLIB_CONFIG_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#define PIXEL_ASPECT_RATIO (4.0f / 5)
#define FPS (15)

typedef enum
{
    PALETTE_ORIGINAL = 0,
    PALETTE_HARSH,
    PALETTE_GRAY
} palette_t;

typedef struct {
    palette_t palette;
    bool show_3310_fps;
} config_t;


extern SDL_Surface *surface;

void sdl2_set_config(config_t config);

#endif /* RAYLIB_CONFIG_H */
