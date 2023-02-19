#ifndef TILEMAP_H
#define TILEMAP_H

#include <engine/engine.h>

#define TILE_W 4
#define TILE_H 4

#define TILEMAP_WIDTH 20
#define TILEMAP_HEIGHT 20

typedef enum
{
    TILE_EMPTY = 0,
    TILE_WALL,
    TILE_SNAKE_HEAD_UP,
    TILE_SNAKE_HEAD_DOWN,
    TILE_SNAKE_HEAD_LEFT,
    TILE_SNAKE_HEAD_RIGHT,
    TILE_SNAKE_HEAD_OPEN_UP,
    TILE_SNAKE_HEAD_OPEN_DOWN,
    TILE_SNAKE_HEAD_OPEN_LEFT,
    TILE_SNAKE_HEAD_OPEN_RIGHT,
    TILE_SNAKE_BODY_UP,
    TILE_SNAKE_BODY_DOWN,
    TILE_SNAKE_BODY_LEFT,
    TILE_SNAKE_BODY_RIGHT,
    TILE_SNAKE_BODY_FULL_UP,
    TILE_SNAKE_BODY_FULL_DOWN,
    TILE_SNAKE_BODY_FULL_LEFT,
    TILE_SNAKE_BODY_FULL_RIGHT,
    TILE_SNAKE_CORNER_TOPLEFT,
    TILE_SNAKE_CORNER_TOPRIGHT,
    TILE_SNAKE_CORNER_BOTTOMLEFT,
    TILE_SNAKE_CORNER_BOTTOMRIGHT,
    TILE_SNAKE_CORNER_FULL_TOPLEFT,
    TILE_SNAKE_CORNER_FULL_TOPRIGHT,
    TILE_SNAKE_CORNER_FULL_BOTTOMLEFT,
    TILE_SNAKE_CORNER_FULL_BOTTOMRIGHT,
    TILE_SNAKE_TAIL_UP,
    TILE_SNAKE_TAIL_DOWN,
    TILE_SNAKE_TAIL_LEFT,
    TILE_SNAKE_TAIL_RIGHT
} tile_t;

typedef struct tilemap_t_
{
    uchar width;
    uchar height;
    tile_t *tile_data;
} tilemap_t;

bool is_snake_tile(tile_t tile);

/**
 * @brief
 *
 * @param x - X in screen coords (i.e pixels)
 * @param y - Y in screen coords (i.e pixels)
 * @param tile
 */
void draw_tile(char x, char y, tile_t tile);

/**
 * @brief
 *
 * @param cam_x Camera X position in pixels
 * @param cam_y Camera Y position in pixels
 * @param tilemap
 */
void draw_tilemap(char cam_x, char cam_y, const tilemap_t *tilemap);

#endif /* TILEMAP_H */
