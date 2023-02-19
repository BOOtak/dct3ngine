#ifndef ENTITY_H
#define ENTITY_H

#include <engine/engine.h>

typedef enum
{
    DIR_LEFT,
    DIR_RIGHT
} dir_t;

typedef enum
{
    STATE_STAND,
    STATE_RUN,
    STATE_ATTACK
} state_t;

typedef struct entity_t_
{
    char x;
    char y;
    dir_t dir;
    state_t state;
    void (*init)(struct entity_t_ *entity);
    void (*update)(struct entity_t_ *entity);
    void (*draw)(const struct entity_t_ *entity);
} entity_t;

#endif /* ENTITY_H */
