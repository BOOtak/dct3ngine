#include <engine/engine.h>

#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "raylib.h"
#include "raylib_config.h"
#include "../engine_internal.h"

const int screen_height = 48;
const int screen_width = 84;

static Color orig_fg = {.r = 0x43, .g = 0x52, .b = 0x3d, .a = 0xff};
static Color orig_bg = {.r = 0xc7, .g = 0xf0, .b = 0xd8, .a = 0xff};

static Color harsh_fg = {.r = 0x2b, .g = 0x3f, .b = 0x09, .a = 0xff};
static Color harsh_bg = {.r = 0x9b, .g = 0xc7, .b = 0x00, .a = 0xff};

static Color gray_fg = {.r = 0x87, .g = 0x91, .b = 0x88, .a = 0xFF};
static Color gray_bg = {.r = 0x1a, .g = 0x19, .b = 0x14, .a = 0xFF};

static Color fg;
static Color bg;

static uchar *screen_buf;

static double draw_ts;
static bool print_fps = false;

void raylib_set_config(config_t config)
{
    fg = orig_fg;
    bg = orig_bg;

    switch (config.palette)
    {
    case PALETTE_ORIGINAL:
        fg = orig_fg;
        bg = orig_bg;
        break;
    case PALETTE_HARSH:
        fg = harsh_fg;
        bg = harsh_bg;
        break;
    case PALETTE_GRAY:
        fg = gray_fg;
        bg = gray_bg;
        break;
    default:
        break;
    }

    // TODO: implement 3310 fps draw
    print_fps = config.show_3310_fps;
}

void init_engine()
{
    screen_buf = calloc((screen_height * screen_width) >> 3, sizeof(uchar));
}

void close_engine()
{
    free(screen_buf);
}

void begin_draw(bool clear)
{
    if (clear)
    {
        memset(screen_buf, 0, ((screen_width * screen_height) >> 3) * sizeof(uchar));
    }

    draw_ts = GetTime();
}

void end_draw(bool immediate /* ignore for raylib */)
{
    double delta_draw = GetTime() - draw_ts;
    ClearBackground(bg);
    if (print_fps)
    {
        // TODO: calc CPU clock difference, for now hardcode at 400x
        double device_delta = delta_draw * 400;
        printf("dt: %.4f; fps: %.2f\n", device_delta, 1.0 / device_delta);
    }

    for (uchar i = 0; i < screen_width; i++)
    {
        for (uchar j = 0; j < screen_height >> 3; j++)
        {
            uchar check = 1;
            for (uchar px = 0; px < 8; px++)
            {
                if (screen_buf[i + screen_width * j] & check)
                {
                    DrawPixel(i, (j << 3) + px, fg);
                }

                check <<= 1;
            }
        }
    }
}

void draw_pixel(uchar x, uchar y)
{
    screen_buf[x + ((screen_width) * (y >> 3))] |= 1 << (y & 7);
}

uchar *get_screen_buf()
{
    return screen_buf;
}

/**
 * @brief Check whether key is pressed
 * Default mapping (Raylib => Nokia):
 * 1, 2, 3 => 1, 2, 3
 * Q, W, E => 4, 5, 6
 * A, S, D => 7, 8, 9
 * Z, X, C => *, 0, #
 *
 * Up, Down, Left, Right => 2, 8, 4, 6
 *
 * TODO: Add option to set custom mappings?
 *
 * @param key ken on Nokia keypad to check
 * @return int 1 if pressed, 0 otherwise
 */
int is_key_down(key_3310_t key)
{
    switch (key)
    {
    case KEYPAD_1:
        return IsKeyDown(KEY_ONE);
    case KEYPAD_2:
        return IsKeyDown(KEY_TWO) || IsKeyDown(KEY_UP);
    case KEYPAD_3:
        return IsKeyDown(KEY_THREE);
    case KEYPAD_4:
        return IsKeyDown(KEY_Q) || IsKeyDown(KEY_LEFT);
    case KEYPAD_5:
        return IsKeyDown(KEY_W);
    case KEYPAD_6:
        return IsKeyDown(KEY_E) || IsKeyDown(KEY_RIGHT);
    case KEYPAD_7:
        return IsKeyDown(KEY_A);
    case KEYPAD_8:
        return IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN);
    case KEYPAD_9:
        return IsKeyDown(KEY_D);
    case KEYPAD_ASTERISK:
        return IsKeyDown(KEY_Z);
    case KEYPAD_0:
        return IsKeyDown(KEY_X);
    case KEYPAD_SHARP:
        return IsKeyDown(KEY_C);
    default:
        break;
    }

    return 0;
}
