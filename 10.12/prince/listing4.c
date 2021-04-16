/* _Exp function, Plauger exp() core
corrected treatment of negative
 * arguments -TCP 16.IX.91
 */
#include <math.h>
#include "xmath.h"
#include <float.h>

#define hugexp (double)HUGE_EXP
#define invln2 1.4426950408889634074

int _Exp(px, eoff)
double *px;
int eoff;
{
    /* Compute e^(*px)*2^eoff, x finite; eoff would be -1
     * for cosh(), sinh() */

    double y, g;
    int xexp;
    const static float round[] = {.5, -.5};
#if 0 /* works OK but slow on many machines */
    g = xexp = *px * invln2 + round[*px < 0];
#else
    /* VAX requires -(((short *)px)[_D0]>>15) */
    g = xexp = *px * invln2 + round[((unsigned int *) px)[
        _D0] >> 31];
#endif
        g = (*px - g * (22713. / 32768.)) - g *
            1.428606820309417232e-6;
    y = g * g;
#define c1 15132.70094679458800
    g += g * y * (420.30235984886453 / c1 + y * (1 / c1));
#if FLT_RADIX > 10
#error "rescale for better accuracy"
#endif
    *px = .5 + g / (30265.40189358917686 / c1 - g + y *
        (3362.72154416335329 / c1 + y *
            (30.01511290682109 / c1)));
    return _Dscale(px, xexp + eoff + 1);
}
WRAP_EOF
