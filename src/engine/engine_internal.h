#ifndef ENGINE_INTERNAL_H
#define ENGINE_INTERNAL_H

#include <stdbool.h>

void init_engine();

void close_engine();

void begin_draw(bool clear);

void end_draw(bool immediate);

#endif /* ENGINE_INTERNAL_H */
