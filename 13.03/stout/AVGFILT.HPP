//
//  AVGFILT.HPP - Header file for data averaging filter.
//

#include <limits.h>
#include "circbuf.hpp"

class AvgFilt_T : public cbuf_t<short>
{
public:
      short AvgFilt();
};

/************************************************************************/
/*                                                                      */
/*  AvgFilt() - Function to average the data in a circular buffer.      */
/*                                                                      */
/************************************************************************/

short AvgFilt_T::AvgFilt()
{
      size_t i;
      long accum;
      short *obuf  = new short[size * sizeof(short)];

      // cbuf_t::buf may be subject to asynchronous modification,
      //  so take a snapshot of it.

      memcpy(obuf, buf, size * sizeof(short));
      for (i = 0, accum = 0L; i < size; ++i)
            accum += obuf[i];
      return(short)(accum / size);
}


