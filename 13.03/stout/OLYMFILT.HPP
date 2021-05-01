//
//  OLYMFILT.HPP - Header file for 16-bit "Olympic" filter function.
//

#include <limits.h>
#include "circbuf.hpp"

typedef enum {OBUF_TINY = 4, OBUF_SMALL = 6, OBUF_MEDIUM = 10,
              OBUF_LARGE = 18, OBUF_HUGE = 34} OSIZE;

#ifndef __BORLANDC__
 template<class T> max(T a, T b) {return (a > b) ? a : b; };
 template<class T> min(T a, T b) {return (a < b) ? a : b; };
#endif

class OlympicFilt_T : public cbuf_t<short>
{
public:
      OlympicFilt_T(OSIZE);
      short OlympicFilt();
};

/************************************************************************/
/*                                                                      */
/*  OlympicBuf() Circular buffer constructor.                           */
/*                                                                      */
/*  Arguments: 1 - Number of elements in the circular buffer.           */
/*                                                                      */
/************************************************************************/

OlympicFilt_T::OlympicFilt_T(OSIZE len = OBUF_HUGE)
{
      init((size_t)len);
}

/************************************************************************/
/*                                                                      */
/*  OlympicFilt() - Function to perform an "Olympic" filter on the      */
/*                  data in a circular buffer.                          */
/*                                                                      */
/************************************************************************/

short OlympicFilt_T::OlympicFilt()
{
      size_t i;
      long accum;
      short *obuf  = new short[size * sizeof(short)];
      short bufmin = SHRT_MAX, bufmax  = SHRT_MIN;

      // cbuf_t::buf may be subject to asynchronous modification,
      //  so take a snapshot of it.

      memcpy(obuf, buf, size * sizeof(short));
      for (i = 0, accum = 0L; i < size; ++i)
      {
            accum += obuf[i];
            bufmin = min(obuf[i], bufmin);
            bufmax = max(obuf[i], bufmax);
      }
      accum -= bufmin;
      accum -= bufmax;
      switch (size)
      {
      case OBUF_TINY:
            return (short)(accum >> 1);

      case OBUF_SMALL:
            return (short)(accum >> 2);

      case OBUF_MEDIUM:
            return (short)(accum >> 3);

      case OBUF_LARGE:
            return (short)(accum >> 4);

      case OBUF_HUGE:
            return (short)(accum >> 5);

      default:
            return(short)(accum / (size - 2));
      }
}


