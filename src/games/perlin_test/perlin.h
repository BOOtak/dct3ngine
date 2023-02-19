#ifndef PERLIN_H
#define PERLIN_H

#include "fixed.h"

Fixed perlin(Fixed x, Fixed y, Fixed angle, int cache_idx);

void perlin16_fast(int width, int height, Fixed angle);

#endif /* PERLIN_H */
