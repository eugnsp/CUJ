//
//  FILTDEMO.CPP - "Olympic" vs. averaging filter demo
//
//  Performs "Olympic" and averaged filter of command line data
//

#include <iostream.h>
#include "olymfilt.hpp"
#include "avgfilt.hpp"

main(int argc, char *argv[])
{
      int n;
      short *bufptr;
      size_t len;
      OlympicFilt_T     OlyBuf(OBUF_SMALL);
      AvgFilt_T         AvgBuf;

      while (--argc)
      {
            n = atoi(*++argv);
            cout << "Adding " << n << " returned " <<
                  (int)OlyBuf.add(n) << endl;
      }
      cout << "\nOlympic average is " << OlyBuf.OlympicFilt() << endl;
      OlyBuf.export(&bufptr, &len);
      AvgBuf.import(bufptr, len);
      cout << "\nStraight average is " << AvgBuf.AvgFilt() << endl;
      return EXIT_SUCCESS;
}


