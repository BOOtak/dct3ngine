#ifndef BITMAP_H
#define BITMAP_H

#include <engine/engine.h>

typedef struct bitmap_t_ {
    uchar width;
    uchar height;
    uchar* buf;
} bitmap_t;

typedef struct anim_t_ {
    uchar frame_count;
    uchar speed;
    uchar current_frame;
    uchar count;
    bitmap_t* bitmaps;
} anim_t;

extern bitmap_t error_bitmap;

void draw_bitmap(char x, char y, const bitmap_t *bitmap);

void draw_bitmap_ex(char x, char y, const bitmap_t *bitmap, bool flip_x);

void draw_anim(char x, char y, const anim_t *anim);

void update_anim(anim_t* anim);

#endif /* BITMAP_H */
