#ifndef FIXED_H
#define FIXED_H

/* Fixed point library */

#define ONE 0x400
#define FRAC_BITS 10
#define FRAC_MASK 0x000003FF
#define MANT_MASK 0xFFFFFC00
#define SIGN_MASK 0xFFC00000

typedef int Fixed;

#define FIX_MUL(X, Y) ((Fixed)(((X) * (Y)) >> FRAC_BITS))
#define FIX_DIV(X, Y) ((Fixed)(((X) / ((Y) >> FRAC_BITS))))

#define TO_INT(X) ((X) >> FRAC_BITS)
#define FIX(X) ((Fixed)((X) << FRAC_BITS))

#endif /* FIXED_H */
