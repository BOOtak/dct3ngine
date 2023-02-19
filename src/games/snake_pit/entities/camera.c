#include "camera.h"
#include "player.h"
#include "utils.h"

void update_camera(entity_t *cam)
{
    // TODO: follow in box-like style
    cam->x = MAX(0, (player.x - screen_width / 2));
    cam->y = MAX(0, (player.y - screen_height / 2));
}

entity_t camera = {
    .update = update_camera};
