#include "game_screen.h"

#include <stdio.h>
#include <malloc.h>

#include <engine/engine.h>

#include "perlin.h"

extern int randoms;
extern int rotations;
extern int gradients;
extern int interpolations;

Fixed dy_low = FIX(0);
Fixed dy_high = FIX(0);
Fixed angle = FIX(0);

void game_screen_init()
{
    dy_low = FIX(0);
    dy_high = FIX(0);
    angle = FIX(0);
}

screen_t game_screen_update()
{
    randoms = 0;
    rotations = 0;
    gradients = 0;
    interpolations = 0;
    return game_screen;
}

void game_screen_draw()
{
    if (is_key_down(KEYPAD_6))
    {
        angle += FIX(8);
    }
    else if (is_key_down(KEYPAD_4))
    {
        angle -= FIX(8);
    }

    dy_low += ONE / 6;
    dy_high += ONE / 4;
    int cell_size = 16;

    Fixed one_over_screen_height = FIX_DIV(ONE, FIX(screen_height));
    for (int j = 0; j < screen_height; j++)
    {
        for (int i = 0; i < screen_width - 1; i++)
        {
            if (i + j & 1)
            {
                Fixed val_low = perlin(FIX(i) / cell_size, FIX(j) / cell_size + dy_low, angle, 0);
                Fixed mul = FIX_MUL(FIX(j), one_over_screen_height);
                val_low = FIX_MUL(val_low, mul);

                Fixed val_high = perlin(FIX(i) / cell_size * 2, FIX(j) / cell_size * 2 + dy_high, angle, 1);

                Fixed res = (val_low + val_high / 2) * 2 / 3;
                if (res > 350)
                {
                    draw_pixel(i, j);
                }
                if (res > 450)
                {
                    draw_pixel(i, j);
                    draw_pixel(i + 1, j);
                }
            }
        }
    }

    // perlin16_fast(screen_width, screen_height, angle);

    printf("%d %d %d %d\n", randoms, rotations, gradients, interpolations);
}

void game_screen_close()
{
}

screen_t game_screen = {
    .name = 'GAME',
    .init = game_screen_init,
    .update = game_screen_update,
    .draw = game_screen_draw,
    .close = game_screen_close};
