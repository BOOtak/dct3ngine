#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>

#include "screen.h"

typedef unsigned char uchar;

typedef enum
{
    KEYPAD_1 = 0b0000000000000001,
    KEYPAD_2 = 0b0000000000000010,
    KEYPAD_3 = 0b0000000000000100,
    KEYPAD_4 = 0b0000000000001000,
    KEYPAD_5 = 0b0000000000010000,
    KEYPAD_6 = 0b0000000000100000,
    KEYPAD_7 = 0b0000000001000000,
    KEYPAD_8 = 0b0000000010000000,
    KEYPAD_9 = 0b0000000100000000,
    KEYPAD_ASTERISK = 0b0000001000000000,
    KEYPAD_0 = 0b0000010000000000,
    KEYPAD_SHARP = 0b0000100000000000
} key_t;

/**
 * @brief First screen to be processed when the game starts. You should have a
 * definition of this screen in your code in order for the game to work.
 */
extern screen_t game_screen;

extern const int screen_width;
extern const int screen_height;

void draw_pixel(uchar x, uchar y);

uchar *get_screen_buf();

int is_key_down(key_t key);

#endif // ENGINE_H
