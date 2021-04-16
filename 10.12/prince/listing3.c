#include <errno.h>
#include "math.h"
#include <float.h>
#include "xmath.h"
#define twobypi .63661977236758134308

double _Sin(x, qoff)
double x;
unsigned int qoff;
{                               /* sin(x) or cos(x) */
    double g, g4, g2;
    float gr = 1 / DBL_EPSILON;
    int quad;

    if (FLT_ROUNDS > 0) {
        /* Compiler should eliminate dead code if
         * FLT_ROUNDS is compile time constant */
        if ((g = x) < 0)
            gr = -gr;
        if (FLT_ROUNDS == 1)
            g = (g * twobypi + gr) - gr;
        /* ANINT(x*2/pi) */
        else
            g = ((g * twobypi + (FLT_ROUNDS == 2 ?
                        -.5 : .5)) + gr) - gr;
    } else {
        /* assume FLT_ROUNDS ==0 for positive numbers */
        g = x * twobypi;
        g = x < 0 ?
            gr - ((.5 - g) + gr) : ((g + .5) + gr) - gr;
    }
#if 0
    /* Reduce g to [-3,3] by ignoring reductions by 2pi
     * when casting to int, so there won't ever be
     * overflow; if the argument is excessively large, it
     * will be treated as reducing to first or last octant
     * as there is not enough precision remaining to
     * distinguish octants */
    gr *= 4;
    /* qoff = qoff + (g-ANINT(g*4)) */
    if (fabs(g) >= 1 / DBL_EPSILON)
        errno = ERANGE;
    quad = g - (FLT_ROUNDS > 0 || g >= 0 ?
        (g + gr) - gr : gr - (gr - g));
#else
/* much faster on SPARC MIPS and HP but could overflow */
    quad = ((int) g) & 3;
#endif
    /* Get remainder after subtracting nearest multiple of
     * pi/2 */
        g = (x - g * (3294198. / 2097152.)) - g *
            3.139164786504813217e-7;
/* Now calculate +- sin() or cos(), -Pi/4 <= x <= Pi/4 */
    g2 = g * g;
    g4 = g2 * g2;
    if ((qoff += quad) & 1)     /* cosine series */
        g = 1 + g2 * (-.499999999999999994 + g2 *
            (.041666666666666452 + g2 *
                (-.001388888888886110 + g2 *
                    .000024801587283884))
            + g4 * g4 * (-.000000275573130985 + g2 *
                (.000000002087558246 - g2 *
                    .000000000011353383)));
    else                        /* sine series */
        g += g * g2 * (-.16666666666666616 + g2 *
            (.00833333333332036 + g2 *
                (-.00019841269828653 + g2 *
                    .0000027557313377252))
            + g4 * g4 * (-.000000025050717097 + g2 *
                .00000000015894743));
    return qoff & 2? -g: g;
}
WRAP_EOF
