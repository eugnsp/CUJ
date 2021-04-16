#include "xmath.h"
#include <float.h>
#define twopi 6.28318530717958647693
#define twobypi .63661977236758134308

double _Sin(x, qoff)
double x;
unsigned int qoff;
{                               /* sin(x) or cos(x) */
    double g, g2;
    long quad;
    int big;

    if ((big = HUGE_RAD < x) || x < -HUGE_RAD) {
        g = x * (1 / twopi);
        x -= (big ?
            (g + 1 / DBL_EPSILON) - 1 / DBL_EPSILON :
            1 / DBL_EPSILON - (1 / DBL_EPSILON - g)) *
            twopi;
        /* Rounding mode doesn't matter */
    }
    g = quad = x * twobypi + (0 < x ? .5 : -.5);
#if LDBL_DIG > DBL_DIG && 0
        g = x - g * 1.57079632679489661923L;
#else
        g = (x - g * (3294198. / 2097152.)) - g *
            3.139164786504813217e-7;
#endif
    g2 = g * g;
    if ((qoff += (unsigned long) quad & 3) & 1)
        g = 1 + g2 * (-.499999999999999994 + g2 *
            (.041666666666666452 + g2 *
                (-.001388888888886110 + g2 *
                    (.000024801587283884 + g2 *
                        (-.000000275573130985 + g2 *
                            (.000000002087558246 - g2 *
                                .000000000011353383))))));
    else
        g += g * g2 * (-.16666666666666616 + g2 *
            (.00833333333332036 + g2 * (-.00019841269828653
                    + g2 * (.0000027557313377252
                        + g2 * (-.000000025050717097 + g2 *
                            .00000000015894743)))));
    return qoff & 2? -g: g;
}
WRAP_EOF
