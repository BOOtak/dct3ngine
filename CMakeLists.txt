cmake_minimum_required(VERSION 3.0)
project(3310_engine C CXX)

set(CMAKE_CXX_STANDARD 17)

# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

if (NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-multichar")
endif()

if (EMSCRIPTEN)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
endif ()

add_subdirectory(libs/raylib)

# add_subdirectory(libs/raygui/projects/CMake)

add_library(raylib_backend STATIC
    src/engine/raylib/main.c
    src/engine/raylib/raylib_engine.c
    )

target_include_directories(raylib_backend PUBLIC include)

target_link_libraries(raylib_backend PUBLIC raylib)

add_executable(perlin_test
    src/games/perlin_test/game_screen.c
    src/games/perlin_test/perlin.c
    src/games/perlin_test/trig.c
    )

target_link_libraries(perlin_test PUBLIC raylib_backend)

add_executable(snake_pit
    src/games/snake_pit/bitmap.c
    src/games/snake_pit/tilemap.c
    src/games/snake_pit/entities/camera.c
    src/games/snake_pit/entities/player.c
    src/games/snake_pit/screens/game_screen.c
    )

target_include_directories(snake_pit PRIVATE src/games/snake_pit)

target_link_libraries(snake_pit PUBLIC raylib_backend)

target_compile_definitions(perlin_test PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
