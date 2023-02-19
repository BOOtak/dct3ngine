#include "perlin.h"
#include "trig.h"

#include <engine/engine.h>
#include <stdio.h>

typedef struct FixVector2_
{
    Fixed x;
    Fixed y;
} FixVector2;

static int x0_cache[2];
static int y0_cache[2];
static FixVector2 grid_cache[2][2][2];

int randoms = 0;
int rotations = 0;
int gradients = 0;
int interpolations = 0;

float to_float(Fixed fp)
{
    int int_part = fp >> FRAC_BITS;
    int frac_part = fp & FRAC_MASK;
    if (fp & 0x80000000)
    {
        int_part |= SIGN_MASK;
    }

    return (float)int_part + (((float)frac_part) / (1 << FRAC_BITS));
}

FixVector2 randomGradient(int ix, int iy)
{
    // No precomputed gradients mean this works for any number of grid coordinates
    unsigned a = ix, b = iy;
    a *= 3284157443u;
    b ^= a << 16 | a >> 16;
    b *= 1911520717u;
    a ^= b << 16 | b >> 16;
    a *= 2048419325u;
    Fixed random = FIX_MUL((a >> 22), FIX(360)); // in [0, 360]
    FixVector2 v;
    v.x = fix_cos(random);
    v.y = fix_sin(random);
    randoms++;
    return v;
}

// Rotate vector by angle
FixVector2 fix_Vector2Rotate(FixVector2 v, Fixed angle)
{
    FixVector2 result = {0};

    Fixed cosres = fix_cos(angle);
    Fixed sinres = fix_sin(angle);

    result.x = FIX_MUL(v.x, cosres) - FIX_MUL(v.y, sinres);
    result.y = FIX_MUL(v.x, sinres) + FIX_MUL(v.y, cosres);

    rotations++;
    return result;
}

// Calculate two vectors dot product
Fixed fix_Vector2DotProduct(FixVector2 v1, const FixVector2 *v2)
{
    int result = (FIX_MUL(v1.x, v2->x) + FIX_MUL(v1.y, v2->y));
    return result;
}

// Computes the dot product of the distance and gradient vectors.
Fixed dotGridGradient(const FixVector2 *gradient, int ix, int iy, Fixed x, Fixed y)
{
    // Compute the distance vector
    FixVector2 dist;
    dist.x = x - FIX(ix);
    dist.y = y - FIX(iy);

    gradients++;
    Fixed result = fix_Vector2DotProduct(dist, gradient);
    // if (ix == 3 && iy == 2)
    // {
    //     printf("    %d %d: %f %f; %f %f: %f\n", ix, iy, to_float(gradient->x), to_float(gradient->y), to_float(x), to_float(y), to_float(result));
    // }
    return result;
}

// cubic interpolation
Fixed interpolate(Fixed a0, Fixed a1, Fixed w)
{
    interpolations++;
    return FIX_MUL(FIX_MUL(FIX_MUL((a1 - a0), (FIX(3) - FIX_MUL(w, FIX(2)))), w), w) + a0;
    // return a0 + FIX_MUL(w, (a1 - a0));
}

// Compute Perlin noise at coordinates x, y
Fixed perlin(Fixed x, Fixed y, Fixed angle, int cache_idx)
{
    // Determine grid cell coordinates
    int x0 = TO_INT(x);
    int x1 = x0 + 1;
    int y0 = TO_INT(y);
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    Fixed sx = x - FIX(x0);
    Fixed sy = y - FIX(y0);

    // Interpolate between grid point gradients
    Fixed n0, n1, ix0, ix1, value;

    if (x0 != x0_cache[cache_idx] || y0 != y0_cache[cache_idx])
    {
        grid_cache[cache_idx][0][0] = fix_Vector2Rotate(randomGradient(x0, y0), angle);
        grid_cache[cache_idx][0][1] = fix_Vector2Rotate(randomGradient(x0, y1), angle);
        grid_cache[cache_idx][1][0] = fix_Vector2Rotate(randomGradient(x1, y0), angle);
        grid_cache[cache_idx][1][1] = fix_Vector2Rotate(randomGradient(x1, y1), angle);
        x0_cache[cache_idx] = x0;
        y0_cache[cache_idx] = y0;
    }

    n0 = dotGridGradient(&grid_cache[cache_idx][0][0], x0, y0, x, y);
    n1 = dotGridGradient(&grid_cache[cache_idx][1][0], x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(&grid_cache[cache_idx][0][1], x0, y1, x, y);
    n1 = dotGridGradient(&grid_cache[cache_idx][1][1], x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return (value + ONE) / 2;
}

typedef unsigned char uchar;

// Generate 1-bit Perlin noise with cell size = 16
void perlin16_fast(int width, int height, Fixed angle)
{
    FixVector2 cache[2][2];

    Fixed dx = ONE >> 4;
    Fixed dy = ONE >> 4;

    int cell_width = width >> 4;
    int cell_height = height >> 4;
    uchar x0, y0;
    for (y0 = 0; y0 < cell_height; y0++)
    {
        if (y0 == 0)
        {
            cache[1][0] = fix_Vector2Rotate(randomGradient(1, 0), angle);
        }
        else
        {
            cache[1][0] = cache[1][1];
        }

        cache[1][1] = fix_Vector2Rotate(randomGradient(1, y0 + 1), angle);
        for (x0 = 0; x0 < cell_width; x0++)
        {
            if (x0 == 0)
            {
                cache[0][0] = fix_Vector2Rotate(randomGradient(0, y0), angle);
                cache[0][1] = fix_Vector2Rotate(randomGradient(0, y0 + 1), angle);
            }
            else
            {
                cache[0][0] = cache[1][0];
                cache[0][1] = cache[1][1];
            }

            cache[1][0] = fix_Vector2Rotate(randomGradient(x0 + 1, y0), angle);
            cache[1][1] = fix_Vector2Rotate(randomGradient(x0 + 1, y0 + 1), angle);

            uchar x1 = x0 + 1;
            uchar y1 = y0 + 1;

            Fixed x, y;

            int i = x0 << 4;

            // if (x0 == 3 && y0 == 2)
            // {
            //     printf("------- start\n");
            // }

            Fixed n0, n1, ix0, ix1;
            for (x = FIX(x0); x < FIX(x1); x += dx, i++)
            {
                int j = y0 << 4;

                Fixed n0_up_start = dotGridGradient(&cache[0][0], x0, y0, x, FIX(y0));
                Fixed n0_up_end = dotGridGradient(&cache[0][0], x0, y0, x, FIX(y1));
                Fixed d_n0_up = (n0_up_end - n0_up_start) >> 4;
                if (d_n0_up & 0x08000000)
                {
                    d_n0_up |= 0xF0000000;
                }
                Fixed n0_up = n0_up_start;

                Fixed n1_up_start = dotGridGradient(&cache[1][0], x1, y0, x, FIX(y0));
                Fixed n1_up_end = dotGridGradient(&cache[1][0], x1, y0, x, FIX(y1));
                Fixed d_n1_up = (n1_up_end - n1_up_start) >> 4;
                if (d_n1_up & 0x08000000)
                {
                    d_n1_up |= 0xF0000000;
                }
                Fixed n1_up = n1_up_start;

                Fixed n0_low_start = dotGridGradient(&cache[0][1], x0, y1, x, FIX(y0));
                Fixed n0_low_end = dotGridGradient(&cache[0][1], x0, y1, x, FIX(y1));
                Fixed d_n0_low = (n0_low_end - n0_low_start) >> 4;
                if (d_n0_low & 0x08000000)
                {
                    d_n0_low |= 0xF0000000;
                }
                Fixed n0_low = n0_low_start;

                Fixed n1_low_start = dotGridGradient(&cache[1][1], x1, y1, x, FIX(y0));
                Fixed n1_low_end = dotGridGradient(&cache[1][1], x1, y1, x, FIX(y1));
                Fixed d_n1_low = (n1_low_end - n1_low_start) >> 4;
                if (d_n1_low & 0x08000000)
                {
                    d_n1_low |= 0xF0000000;
                }
                Fixed n1_low = n1_low_start;

                Fixed sx = x - FIX(x0);

                Fixed ix0_start = interpolate(n0_up_start, n1_up_start, sx);
                Fixed ix0_end = interpolate(n0_up_end, n1_up_end, sx);
                ix0 = ix0_start;
                Fixed d_ix0 = (ix0_end - ix0_start) >> 4;
                if (d_ix0 & 0x80000000)
                {
                    d_ix0 |= 0xF0000000;
                }

                Fixed ix1_start = interpolate(n0_low_start, n1_low_start, sx);
                Fixed ix1_end = interpolate(n0_low_end, n1_low_end, sx);
                ix1 = ix1_start;
                Fixed d_ix1 = (ix1_end - ix1_start) >> 4;
                if (d_ix1 & 0x80000000)
                {
                    d_ix1 |= 0xF0000000;
                }

                for (y = FIX(y0); y < FIX(y1); y += dy)
                {
                    if ((i + j) & 1)
                    {
                        Fixed sy = y - FIX(y0);

                        // n0 = dotGridGradient(&cache[0][0], x0, y0, x, y);
                        // n1 = dotGridGradient(&cache[1][0], x1, y0, x, y);
                        // ix0 = interpolate(n0, n1, sx);
                        // ix0 = interpolate(n0_up, n1_up, sx);

                        // n0 = dotGridGradient(&cache[0][1], x0, y1, x, y);
                        // n1 = dotGridGradient(&cache[1][1], x1, y1, x, y);
                        // ix1 = interpolate(n0_low, n1_low, sx);

                        Fixed value = (interpolate(ix0, ix1, sy) + ONE) >> 1;

                        if (value > 512)
                        {
                            draw_pixel(i, j);
                            draw_pixel(i + 1, j);
                        }
                        else if (value > 450)
                        {
                            draw_pixel(i, j);
                        }
                    }

                    j++;
                    n0_up += d_n0_up;
                    n1_up += d_n1_up;
                    n0_low += d_n0_low;
                    n1_low += d_n1_low;
                    ix0 += d_ix0;
                    ix1 += d_ix1;
                }

                // if (x0 == 3 && y0 == 2)
                // {
                //     printf("------- end\n");
                // }
            }
        }
    }
}
