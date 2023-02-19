#include <math.h>
#include <stdio.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#include "raylib.h"

#include "engine/screen.h"
#include "engine/engine.h"

#include "raylib_config.h"
#include "../engine_internal.h"

screen_t current_screen;
RenderTexture2D target;

void change_screen(screen_t old, screen_t new)
{
    old.close();
    new.init();
    current_screen = new;
}

void update_screen()
{
    screen_t new_screen = current_screen.update();
    if (new_screen.name != current_screen.name)
    {
        printf("Change screen!\n");
        change_screen(current_screen, new_screen);
    }
}

void UpdateDrawFrame()
{
    update_screen();
    float scale = fmin((float)GetScreenWidth() / screen_width / PIXEL_ASPECT_RATIO, (float)GetScreenHeight() / screen_height);
    BeginTextureMode(target);

    begin_draw(true);
    ClearBackground(WHITE);
    current_screen.draw();
    end_draw(true);

    EndTextureMode();
    BeginDrawing();
    ClearBackground(BLACK); // Clear screen background

    // Draw render texture to screen, properly scaled
    DrawTexturePro(target.texture, (Rectangle){0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height},
                   (Rectangle){(GetScreenWidth() - ((float)screen_width * scale * PIXEL_ASPECT_RATIO)) * 0.5f, (GetScreenHeight() - ((float)screen_height * scale)) * 0.5f,
                               (float)screen_width * scale * PIXEL_ASPECT_RATIO, (float)screen_height * scale},
                   (Vector2){0, 0}, 0.0f, WHITE);
    EndDrawing();
}

int main(int argc, char const *argv[])
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_width * 5 * PIXEL_ASPECT_RATIO, screen_height * 5, "3310 engine");

    target = LoadRenderTexture(screen_width, screen_height);
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT); // Texture scale filter to use

    current_screen = game_screen;
    current_screen.init();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 15, 1);
#else
    SetTargetFPS(15);
    //--------------------------------------------------------------------------------------

    init_engine();

    // TODO: move to game logic?
    config_t config = {.palette = PALETTE_HARSH, .show_3310_fps = true};
    raylib_set_config(config);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    current_screen.close();
    close_engine();
    CloseWindow();

    return 0;
}
