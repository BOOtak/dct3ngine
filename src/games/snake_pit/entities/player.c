#include "bitmap.h"
#include "camera.h"
#include "player.h"

uchar knight_buf_st[] = {
    0b11100001,
    0b11000010,
    0b11100100,
    0b11111000,
    0b11100000,
    0b11100000,
    0b10100000,
    0b10100000,
    0b10100000};

uchar knight_buf_run[][9] = {
    {0b11100001,
     0b11000010,
     0b11100100,
     0b11111000,
     0b11100000,
     0b11100000,
     0b10100000,
     0b10100000,
     0b00100000},
    {0b11100001,
     0b11000010,
     0b11100100,
     0b11111000,
     0b11100000,
     0b11100000,
     0b10100000,
     0b10100000,
     0b10000000}};

bitmap_t knight_bitmap = {.width = 8, .height = 9, .buf = knight_buf_st};

bitmap_t knight_run_bitmaps[] = {{.width = 8,
                                  .height = 9,
                                  .buf = knight_buf_run[0]},
                                 {.width = 8,
                                  .height = 9,
                                  .buf = knight_buf_run[1]}};

anim_t knight_run_anim = {
    .speed = 2,
    .bitmaps = knight_run_bitmaps,
    .frame_count = 2,
    .count = 0,
    .current_frame = 0};

void init_player(entity_t *player_entity)
{
    player_entity->x = SPAWN_X;
    player_entity->y = SPAWN_Y;
    player_entity->dir = DIR_LEFT;
    player_entity->state = STATE_STAND;
}

void draw_player(const entity_t *player_entity)
{
    bitmap_t *to_draw;
    switch (player_entity->state)
    {
    case STATE_STAND:
        to_draw = &knight_bitmap;
        break;

    case STATE_RUN:
        to_draw = &knight_run_anim.bitmaps[knight_run_anim.current_frame];
        break;

    default:
        to_draw = &error_bitmap;
        break;
    }

    bool flip_x = false;
    char offset_x = 0;
    if (player_entity->dir == DIR_LEFT)
    {
        flip_x = true;
        offset_x = 4;
    }

    draw_bitmap_ex(player_entity->x - offset_x - camera.x, player_entity->y - camera.y, to_draw, flip_x);
}

void update_player(entity_t *player_entity)
{
    player_entity->state = STATE_RUN;
    if (is_key_down(KEYPAD_4))
    {
        player_entity->x -= 1;
        player_entity->dir = DIR_LEFT;
    }
    else if (is_key_down(KEYPAD_6))
    {
        player_entity->x += 1;
        player_entity->dir = DIR_RIGHT;
    }
    else if (is_key_down(KEYPAD_2))
    {
        player_entity->y -= 1;
    }
    else if (is_key_down(KEYPAD_8))
    {
        player_entity->y += 1;
    }
    else
    {
        player_entity->state = STATE_STAND;
    }

    if (player_entity->state == STATE_RUN)
    {
        update_anim(&knight_run_anim);
    }
    else
    {
        knight_run_anim.current_frame = 0;
    }
}

entity_t player = {
    .init = init_player,
    .update = update_player,
    .draw = draw_player
};
