#ifndef RAYLIB_CONFIG_H
#define RAYLIB_CONFIG_H

#include <stdbool.h>

#define PIXEL_ASPECT_RATIO (4.0f / 5)

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

void raylib_set_config(config_t config);

#endif /* RAYLIB_CONFIG_H */
