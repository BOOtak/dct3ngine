#ifndef SCREEN_H
#define SCREEN_H

struct screen_t_;

typedef struct screen_t_
{
    int name;
    void (*init)();
    struct screen_t_ (*update)();
    void (*draw)();
    void (*close)();
} screen_t;

#endif /* SCREEN_H */
