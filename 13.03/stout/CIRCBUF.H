/*
**  CIRCBUF.H - Header file for circular buffer C functions
*/

#ifndef _CIRCBUF_DEFINED_
#define _CIRCBUF_DEFINED_

typedef enum {CSIZ_TINY = 4, CSIZ_SMALL = 6, CSIZ_MEDIUM = 10,
              CSIZ_LARGE = 18, CSIZ_HUGE = 34} CSIZE;

typedef enum {FALSE, TRUE} LOGICAL;

typedef struct {
      CSIZE    len;
      size_t   next;
      LOGICAL  full;
      short   *buf;
} CBUF;

CBUF    *cbuf_malloc(CSIZE);
LOGICAL  cbuf_add(CBUF *, short);

short    OlympicFilt(CBUF *);
short    AverageFilt(CBUF *);

#ifndef MAX
 #define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
 #define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#endif // _CIRCBUF_DEFINED_


