#include <engine/engine.h>

#include <string.h>

#include "entity.h"
#include "entities/camera.h"
#include "entities/player.h"
#include "game_screen.h"
#include "bitmap.h"
#include "tilemap.h"

tilemap_t game_tilemap;

tile_t game_tile_data[TILEMAP_HEIGHT * TILEMAP_WIDTH];

static void init()
{
    player.init(&player);

    // TODO: generate game map
    memset(&game_tile_data, 0, sizeof(game_tile_data));
    game_tile_data[0] = TILE_WALL;
    game_tile_data[1] = TILE_WALL;
    game_tile_data[2] = TILE_WALL;
    game_tile_data[TILEMAP_WIDTH-1] = TILE_WALL;
    game_tile_data[TILEMAP_WIDTH] = TILE_WALL;
    game_tile_data[TILEMAP_WIDTH * 2] = TILE_WALL;
    game_tile_data[TILEMAP_WIDTH * 3] = TILE_SNAKE_HEAD_UP;

    game_tilemap.width = TILEMAP_WIDTH;
    game_tilemap.height = TILEMAP_HEIGHT;
    game_tilemap.tile_data = game_tile_data;
}

static screen_t update()
{
    player.update(&player);
    camera.update(&camera);

    return game_screen;
}

void draw()
{
    draw_tilemap(camera.x, camera.y, &game_tilemap);
    player.draw(&player);
}

void close()
{
}

screen_t game_screen = {
    .name = 'GAME',
    .init = init,
    .update = update,
    .draw = draw,
    .close = close};
