/*
**  CIRCBUF.C - Circular buffer C functions
*/

#include <stdlib.h>
#include <limits.h>
#include "circbuf.h"

/************************************************************************/
/*                                                                      */
/*  cbuf_malloc() - Function to allocate a circular buffer on the heap. */
/*                                                                      */
/*  Arguments: 1 - Size of buffer.                                      */
/*                                                                      */
/*  Returns: Pointer to buffer struct or NULL upon failure.             */
/*                                                                      */
/************************************************************************/

CBUF *cbuf_malloc(CSIZE size)
{
      CBUF *ptr;

      if (NULL != (ptr = calloc(sizeof(CBUF) + (size * sizeof(short)), 1)))
      {
            ptr->buf  = (short *)((char *)ptr + sizeof(CBUF));
            ptr->len  = size;
            return ptr;
      }
      else  return NULL;
}

/************************************************************************/
/*                                                                      */
/*  cbuf_add() - Function to add data to a circular buffer.             */
/*                                                                      */
/*  Arguments: 1 - Pointer to circular buffer struct.                   */
/*             2 - Data to add.                                         */
/*                                                                      */
/*  Returns: TRUE if buffer has been filled, else FALSE.                */
/*                                                                      */
/************************************************************************/

LOGICAL  cbuf_add(CBUF *cbuf, short data)
{
      cbuf->buf[cbuf->next] = data;
      if (cbuf->len <= ++(cbuf->next))
      {
            cbuf->next = 0;
            cbuf->full = TRUE;
      }
      return cbuf->full;
}

/************************************************************************/
/*                                                                      */
/*  OlympicFilt() - Function to perform an "Olympic" filter on the      */
/*                  data in a circular buffer.                          */
/*                                                                      */
/*  Arguments: 1 - Pointer to circular buffer struct.                   */
/*                                                                      */
/*  Returns: TRUE if buffer has been filled, else FALSE.                */
/*                                                                      */
/************************************************************************/

int OlympicFilt(CBUF * cbuf)
{
      size_t i;
      long accum;
      short cmin  = SHRT_MAX, cmax  = SHRT_MIN;
      short obuf[CSIZ_HUGE * sizeof(short)];

      /*
      **  The buffer may be subject to asynchronous modification,
      **  so take a snapshot of it.
      */

      memcpy(obuf, cbuf->buf, cbuf->len * sizeof(short));

      for (i = 0, accum = 0L; i < cbuf->len; ++i)
      {
            accum += obuf[i];
            cmin   = MIN(obuf[i], cmin);
            cmax   = MAX(obuf[i], cmax);
      }
      accum -= cmin;
      accum -= cmax;
      switch (cbuf->len)
      {
      case CSIZ_TINY:
            return (short)(accum >> 1);

      case CSIZ_SMALL:
            return (short)(accum >> 2);

      case CSIZ_MEDIUM:
            return (short)(accum >> 3);

      case CSIZ_LARGE:
            return (short)(accum >> 4);

      case CSIZ_HUGE:
            return (short)(accum >> 5);

      default:
            return(short)(accum / (cbuf->len - 2));
      }
}

/************************************************************************/
/*                                                                      */
/*  AverageFilt() - Function to filter the data in a circular buffer    */
/*                  by simple averaging.                                */
/*                                                                      */
/*  Arguments: 1 - Pointer to circular buffer struct.                   */
/*                                                                      */
/*  Returns: TRUE if buffer has been filled, else FALSE.                */
/*                                                                      */
/************************************************************************/

short AverageFilt(CBUF * cbuf)
{
      size_t i;
      long accum;
      short obuf[CSIZ_HUGE * sizeof(short)];

      /*
      **  The buffer may be subject to asynchronous modification,
      **  so take a snapshot of it.
      */

      memcpy(obuf, cbuf->buf, cbuf->len * sizeof(short));

      for (i = 0, accum = 0L; i < cbuf->len; ++i)
            accum += obuf[i];

      return(short)(accum / (cbuf->len));
}

#ifdef TEST

#include <stdio.h>

int main(int argc, char *argv[])
{
      int i, n;
      CBUF *cbuf = cbuf_malloc(CSIZ_SMALL);

      while (--argc)
      {
            n = atoi(*++argv);
            printf("Adding %d returned %d\n", n, cbuf_add(cbuf, n));
      }
      printf("\nOlympic average is %d\n", OlympicFilt(cbuf));
      printf("\nStraight average is %d\n", AverageFilt(cbuf));
      return EXIT_SUCCESS;
}

#endif


