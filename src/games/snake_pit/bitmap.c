#include "bitmap.h"

#include <engine/engine.h>

uchar chess_board_buf[] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};

bitmap_t error_bitmap = {
    .width = 8,
    .height = 8,
    .buf = chess_board_buf};

void draw_bitmap(char x, char y, const bitmap_t *bitmap)
{
    draw_bitmap_ex(x, y, bitmap, false);
}

void draw_bitmap_ex(char x, char y, const bitmap_t *bitmap, bool flip_x)
{
    uchar i, j;
    uchar width_in_bytes = ((bitmap->width - 1) >> 3) + 1;
    for (j = 0; j < bitmap->height; j++)
    {
        char draw_y = y + j;
        if (draw_y < 0)
        {
            continue;
        }

        if (draw_y >= screen_height)
        {
            return;
        }

        for (i = 0; i < bitmap->width; i++)
        {
            char draw_x = x + i;
            if (draw_x < 0)
            {
                continue;
            }

            if (draw_x >= screen_width)
            {
                break;
            }

            if (flip_x)
            {
                uchar check_bit = (bitmap->width - i - 1);
                if (bitmap->buf[j * width_in_bytes + (check_bit >> 3)] & (0b10000000 >> (check_bit & 7)))
                {
                    draw_pixel((uchar)draw_x, (uchar)draw_y);
                }
            }
            else
            {
                if (bitmap->buf[j * width_in_bytes + (i >> 3)] & (0b10000000 >> (i & 7)))
                {
                    draw_pixel((uchar)draw_x, (uchar)draw_y);
                }
            }
        }
    }
}

void draw_anim(char x, char y, const anim_t *anim)
{
    draw_bitmap(x, y, &(anim->bitmaps[anim->current_frame]));
}

void update_anim(anim_t *anim)
{
    if (anim->count <= 0)
    {
        anim->count = anim->speed;
        anim->current_frame++;
        if (anim->current_frame >= anim->frame_count)
        {
            anim->current_frame = 0;
        }
    }
    anim->count--;
}
