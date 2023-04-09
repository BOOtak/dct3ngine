cmake_minimum_required(VERSION 3.0)
project(3310_engine C CXX)

set(CMAKE_CXX_STANDARD 17)

# Use raylib default backend
if (NOT DEFINED BACKEND)
    set(BACKEND "RAYLIB")
endif()

# Setting parameters for raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

if (NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-multichar")
endif()

if (EMSCRIPTEN)
    if (BACKEND STREQUAL "SDL2")
       set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_SDL=2")
    else()
       set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
    endif()
endif()

# add_subdirectory(libs/raygui/projects/CMake)

if(BACKEND STREQUAL "RAYLIB")
    add_subdirectory(libs/raylib)
    add_library(raylib_backend STATIC src/engine/raylib/main.c src/engine/raylib/raylib_engine.c)
    target_include_directories(raylib_backend PUBLIC include)
    target_link_libraries(raylib_backend PUBLIC raylib)
elseif(BACKEND STREQUAL "SDL2")
    find_package(SDL2 REQUIRED)
    add_library(sdl2_backend STATIC src/engine/sdl2/main.c src/engine/sdl2/sdl2_engine.c)
    target_include_directories(sdl2_backend PUBLIC include ${SDL2_INCLUDE_DIRS})
    target_link_libraries(sdl2_backend PUBLIC ${SDL2_LIBRARIES})
endif()

add_executable(perlin_test
    src/games/perlin_test/game_screen.c
    src/games/perlin_test/perlin.c
    src/games/perlin_test/trig.c
    )

add_executable(snake_pit
    src/games/snake_pit/bitmap.c
    src/games/snake_pit/tilemap.c
    src/games/snake_pit/entities/camera.c
    src/games/snake_pit/entities/player.c
    src/games/snake_pit/screens/game_screen.c
    )

target_include_directories(snake_pit PRIVATE src/games/snake_pit)

if(BACKEND STREQUAL "RAYLIB")
    target_link_libraries(perlin_test PUBLIC raylib_backend)
    target_link_libraries(snake_pit PUBLIC raylib_backend)
elseif(BACKEND STREQUAL "SDL2")
    target_link_libraries(perlin_test PUBLIC sdl2_backend)
    target_link_libraries(snake_pit PUBLIC sdl2_backend)
endif()

target_compile_definitions(perlin_test PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
