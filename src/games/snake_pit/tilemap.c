#include <stdbool.h>

#include "bitmap.h"
#include "tilemap.h"
#include "utils.h"

const static tile_t tile_snake_first = TILE_SNAKE_HEAD_UP;
const static tile_t tile_snake_last = TILE_SNAKE_TAIL_RIGHT;

uchar tilemap[][TILE_H] = {
    {
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
    },
    {0b11111111,
     0b11111111,
     0b11111111,
     0b11111111},
    {0b00000000,
     0b01100000,
     0b01100000,
     0b10100000}
    // TODO: add more
};

static bitmap_t tile_bitmap = {
    TILE_W,
    TILE_H,
    0};

bool is_snake_tile(tile_t tile)
{
    return tile >= tile_snake_first && tile <= tile_snake_last;
}

void draw_tile(char x, char y, tile_t tile)
{
    // TODO: add all the tiles to the tilemap, now draw just full & empty
    // TODO: cramp 2 4x4 tiles into 4-byte bitmap?
    if (tile >= ARR_SIZE(tilemap))
    {
        return;
    }

    tile_bitmap.buf = tilemap[tile];
    draw_bitmap(x, y, &tile_bitmap);
}

void draw_tilemap(char cam_x, char cam_y, const tilemap_t *tilemap)
{
    if (cam_x >= tilemap->width * TILE_W || cam_y >= tilemap->height * TILE_H)
    {
        return;
    }

    uchar i, j, x, y;
    // clamp camera pos in [0; (screen_width - cam_x)], same for camera_y
    uchar clamp_x = MIN((screen_width - cam_x), MAX(0, cam_x));
    uchar clamp_y = MIN((screen_height - cam_y), MAX(0, cam_y));

    // pray for it to be optimized into bit shifts
    uchar init_i = clamp_x / TILE_W, init_j = clamp_y / TILE_H;
    uchar off_x = clamp_x % TILE_W, off_y = clamp_y % TILE_H;

    for (j = init_j, y = 0; j < tilemap->height; j++)
    {
        for (i = init_i, x = 0; i < tilemap->width; i++)
        {
            draw_tile(x - off_x, y - off_y, tilemap->tile_data[j * tilemap->width + i]);

            x += TILE_W;
            if (x >= screen_width)
            {
                break;
            }
        }

        y += TILE_H;
        if (y >= screen_height)
        {
            return;
        }
    }
}
