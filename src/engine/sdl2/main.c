#include "engine/screen.h"
#include "engine/engine.h"

#include "sdl2_config.h"
#include "../engine_internal.h"

SDL_Surface *video;
SDL_Renderer *render;
SDL_Texture *texture;

screen_t current_screen;

void change_screen(screen_t old, screen_t new) {
	old.close();
	new.init();
	current_screen = new;
}

void update_screen(void) {
	screen_t new_screen = current_screen.update();
	if (new_screen.name != current_screen.name) {
		printf("Change screen!\n");
		change_screen(current_screen, new_screen);
	}
}

void UpdateDrawFrame(void) {
	SDL_FillRect(surface, NULL, 0);
	update_screen();

	begin_draw(true);
	current_screen.draw();
	end_draw(true);

	SDL_BlitSurface(surface, NULL, video, NULL);
	SDL_UpdateTexture(texture, NULL, video->pixels, video->pitch);
	SDL_RenderCopy(render, texture, NULL, NULL);
}

int main(int argc, char *argv[]) {
	int running = 1;
	SDL_Event event;

	SDL_Window *window = SDL_CreateWindow(
		"3310 Engine (SDL)",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width * 5 * PIXEL_ASPECT_RATIO, screen_height * 5,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);
	if (window == NULL) {
		SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (render == NULL) {
		SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	video = SDL_CreateRGBSurface(0, screen_width, screen_height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	if (video == NULL) {
		SDL_Log("SDL_CreateRGBSurface (video) failed: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	surface = SDL_CreateRGBSurface(0, screen_width, screen_height, 8, 0x00, 0x00, 0x00, 0x00);
	if (surface == NULL) {
		SDL_Log("SDL_CreateRGBSurface (surface) failed: %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, screen_width, screen_height);
	if (texture == NULL) {
		SDL_Log("SDL_CreateTexture failed: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	current_screen = game_screen;
	current_screen.init();
	init_engine();

	config_t config = { .palette = PALETTE_HARSH, .show_3310_fps = true };
	sdl2_set_config(config);

	while (running) {
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode) {
						case SDL_SCANCODE_ESCAPE:
							running = 0;
							break;
						case SDL_SCANCODE_F1:
							config.palette = PALETTE_ORIGINAL;
							sdl2_set_config(config);
							break;
						case SDL_SCANCODE_F2:
							config.palette = PALETTE_HARSH;
							sdl2_set_config(config);
							break;
						case SDL_SCANCODE_F3:
							config.palette = PALETTE_GRAY;
							sdl2_set_config(config);
							break;
						case SDL_SCANCODE_F4:
							config.show_3310_fps = !config.show_3310_fps;
							sdl2_set_config(config);
							break;
						default:
							break;
					}
					break;
				case SDL_QUIT:
					running = 0;
					break;
				default:
					break;
			}
		}
		UpdateDrawFrame();
		SDL_RenderPresent(render);
		SDL_Delay(1000 / FPS);
	}

	close_engine();

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(video);
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);

	return EXIT_SUCCESS;
}
