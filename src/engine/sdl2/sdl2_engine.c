#include <engine/engine.h>

#include "sdl2_config.h"

#include <SDL2/SDL.h>

const int screen_height = 48;
const int screen_width = 84;

static SDL_Color orig_palette[] = {
	{.r = 0xc7, .g = 0xf0, .b = 0xd8, .a = 0xff}, /* background */
	{.r = 0x43, .g = 0x52, .b = 0x3d, .a = 0xff}  /* foreground */
};

static SDL_Color harsh_palette[] = {
	{.r = 0x9b, .g = 0xc7, .b = 0x00, .a = 0xff}, /* background */
	{.r = 0x2b, .g = 0x3f, .b = 0x09, .a = 0xff}  /* foreground */
};

static SDL_Color gray_palette[] = {
	{.r = 0x87, .g = 0x91, .b = 0x88, .a = 0xFF},  /* background */
	{.r = 0x1a, .g = 0x19, .b = 0x14, .a = 0xFF}   /* foreground */
};

SDL_Surface *surface = NULL;

static double draw_ts;
static bool print_fps = false;

void sdl2_set_config(config_t config) {
	SDL_SetPaletteColors(surface->format->palette, orig_palette, 0, sizeof(orig_palette));

	switch (config.palette) {
		case PALETTE_ORIGINAL:
			SDL_SetPaletteColors(surface->format->palette, orig_palette, 0, sizeof(orig_palette));
			break;
		case PALETTE_HARSH:
			SDL_SetPaletteColors(surface->format->palette, harsh_palette, 0, sizeof(harsh_palette));
			break;
		case PALETTE_GRAY:
			SDL_SetPaletteColors(surface->format->palette, gray_palette, 0, sizeof(gray_palette));
			break;
		default:
			break;
	}

	// TODO: implement 3310 fps draw
	print_fps = config.show_3310_fps;
}

void init_engine() {

}

void close_engine() {

}

void begin_draw(bool clear) {
	if (clear) {
		SDL_FillRect(surface, NULL, 0x00); /* 0 is background */
	}

	draw_ts = (double) SDL_GetTicks() / 1000;
}

void end_draw(bool immediate /* ignore for sdl2 */) {
	double delta_draw = ((double) SDL_GetTicks() / 1000) - draw_ts;
	if (print_fps) {
		// TODO: calc CPU clock difference, for now hardcode at 400x
		double device_delta = delta_draw * 400;
		printf("dt: %.4f; fps: %.2f\n", device_delta, 1.0 / device_delta);
	}
}

void draw_pixel(uchar x, uchar y) {
	*(((Uint8 *) surface->pixels) + (y * surface->pitch)  + (x * sizeof(Uint8))) = 0x01 & 0xFF; /* 1 is foreground */
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
int is_key_down(key_3310_t key) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	switch (key) {
		case KEYPAD_1:
			return keys[SDL_SCANCODE_1];
		case KEYPAD_2:
			return keys[SDL_SCANCODE_2] || keys[SDL_SCANCODE_UP];
		case KEYPAD_3:
			return keys[SDL_SCANCODE_3];
		case KEYPAD_4:
			return keys[SDL_SCANCODE_Q] || keys[SDL_SCANCODE_LEFT];
		case KEYPAD_5:
			return keys[SDL_SCANCODE_W];
		case KEYPAD_6:
			return keys[SDL_SCANCODE_E] || keys[SDL_SCANCODE_RIGHT];
		case KEYPAD_7:
			return keys[SDL_SCANCODE_A];
		case KEYPAD_8:
			return keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN];
		case KEYPAD_9:
			return keys[SDL_SCANCODE_D];
		case KEYPAD_ASTERISK:
			return keys[SDL_SCANCODE_Z];
		case KEYPAD_0:
			return keys[SDL_SCANCODE_X];
		case KEYPAD_SHARP:
			return keys[SDL_SCANCODE_C];
		default:
			break;
	}

	return 0;
}
