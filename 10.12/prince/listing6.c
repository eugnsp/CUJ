/* Copyright (c) 1985 Regents of the University of California.

  Use and reproduction of this software are granted in accordance with
  the terms and conditions specified in the Berkeley Software License
  Agreement (in particular, this entails acknowledgement of the
  programs' source, and inclusion of this notice) with the additional
  understanding that all recipients should regard themselves as
  participants in an ongoing research project and hence should feel
  obligated to report their experiences (good or bad) with these
  elementary function codes, using "sendbug 4bsd-bugs@BERKELEY", to the
  authors.

 */

#ifndef lint
static char sccsid[] = "@(#)atan2.c     (T Prince) 1/3/92";
#endif                          /* not lint */

/* ATAN2(Y,X) RETURN ARG (X+iY)
 * DOUBLE PRECISION (VAX D format 56 bits, IEEE DOUBLE 53 BITS)
 * CODED IN C BY K.C. NG, 1/8/85;
 * REVISED BY K.C. NG on 2/7/85, 2/13/85, 3/7/85, 3/30/85, 6/29/85.
 * simplified by T C Prince 12/91
 *
 * Method :
 1. Reduce y to positive by atan2(y,x)=-atan2(-y,x).
 2. Reduce x to positive by (if x and y are unexceptional):
                ARG (x+iy) = arctan(y/x)           ... if x > 0,
                ARG (x+iy) = pi - arctan[y/(-x)]   ... if x < 0,
 3.  According to the integer k=4t+0.25 truncated, t=y/x, the argument
        is further reduced to one of the following intervals and the
        arctangent of y/x is evaluated by the corresponding formula:

        [0,7/16]        atan(y/x) = t - t^3*(a1+t^2*(a2+...a10+t^2*a11)
        [7/16,11/16]    atan(y/x) = atan(1/2) + atan( (y-x/2)/(x+y/2) )
        [11/16.19/16]   atan(y/x) = atan( 1 ) + atan( (y-x)/(x+y) )
        [19/16,39/16]   atan(y/x) = atan(3/2) + atan((y-1.5x)/(x+1.5y))
        [39/16,INF]     atan(y/x) = atan(INF) + atan( -x/y )

  The decimal values may be used, provided that the compiler will
  convert from decimal to binary accurately enough.

 */

const static double
 athfhi = .46364760900080609352, athflo = 4.6249969567426939759E-18,
 at1fhi = .98279372324732905408, at1flo = -2.4407677060164810007E-17,
 PI = 3.14159265358979323846,
/* Chebyshev economized coefficients, 56 bit precision */
 a1 = .33333333333332713, a2 = -.19999999999844163,
 a3 = .14285714270355533, a4 = -.11111110327242694,
 a5 = .09090885408512523, a6 = -.0769185192745614,
 a7 = .06660850184641357, a8 = -.05832239923826337,
 a9 = .04971908607172078, a10 = -.03642599200325829,
 a11 = .01618847031840557;

#include <math.h>

double (atan) (x)
double x;
{
    return atan2(x, 1.);
}

double atan2(y, x)
double y, x;
{
    double t, z, signy, hi, lo, z2;
    int k, m, signx;

    /* Copy down the sign of y and x */
    signy = 1.;
    if( y < 0 ){y = -y;signy= -1.;}
    if(signx = x < 0 ) x = -x;

    if ((t=y/x) < 2.4375) {
        /* Truncate 4(t+1/16) to integer for branching */
        switch (k = t*4 + 0.25) {

        case 0:
        case 1: /* T is in [0,7/16] */
            hi = 0;
            lo = 0;
            break;

        case 2: /* T is in [7/16,11/16] */
            t = (y * 2 - x) / (x * 2 + y);
            hi = athfhi;
            lo = athflo;
            break;

        case 3:
        case 4: /* T is in [11/16,19/16] */
            t = (y - x) / (x + y);
            hi = 51471 / 65536.;
            lo = .00001303156151080961566;
            break;

        default: /* T is in [19/16,39/16] */
            t = (y - x * 1.5) / (x + y * 1.5);
            hi = at1fhi;
            lo = at1flo;
            break;
        }
    } /* End of if (t < 2.4375) */

    else { /* T >= 2.4375 */
/* 0/0 produces NaN (agrees with IEEE, not UNIX tradition) */
        t = -x / y;
        hi = 51471 / 32768.;
        lo = .00001303156151080961566 * 2;
    }

    z = t * t;
    z2 = z * z;
    z = ((lo - t * z *
            (a1 + z * (a2 + z * (a3 + z * (a4 + z * a5))
                    + z2 * z2 * (a6 + z * (a7 + z * (a8
                        + z * (a9 + z * (a10 + z * a11)))))))
        ) + t) + hi;

    return ((signx ? PI - z : z) * signy);
}
