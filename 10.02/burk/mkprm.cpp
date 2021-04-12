/*
(C) Copyright 1990 Ron Burk
All Rights Reserved

mkprm.c - Generate htabp.c, a list of prime numbers.

    This program generates a logarithmic table of prime numbers.  The
table is stored as source code that can be compiled into another program.

 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned long   ulong;

class outfile
    {
    FILE    *fout;      // handle to output file
    int     nprimes;
    void    printhead();
    void    printtail();
public:
    outfile() : fout(NULL), nprimes(0) { };
   ~outfile()                   { close(); }
    void    open(const char *fname);
    void    close()             { printtail(); fclose(fout); }
    void    prime(ulong);       // format prime # into C initialized array
    };


//      PERCENTAGE specifies the allowable percentage discrepancy between
//  the desired table size and the nearest prime number in the table. 

const PERCENTAGE = 20;


//      MAX_PRIME limits the range of numbers which will be searched for
//  primes.  Note that USHRT_MAX is guaranteed to be >= 65,535

const unsigned MAX_PRIME = USHRT_MAX;


const MAX_NPRIMES = 100 + MAX_PRIME / 10;

const char OUTFILE[] = "htabp.c";

int     main(int /*argc*/, char /***argv*/)
    {
    unsigned previous = 0, log = 10;
    static unsigned primes[MAX_NPRIMES];
    int     nprimes = 1;
    int     nprint  = 0;
    outfile fout;

    fout.open(OUTFILE);
    primes[0]   = 2;
    for(unsigned num = 3; num < MAX_PRIME-log && nprimes < MAX_NPRIMES; num+=2)
        {
        for(int i = 0; i < nprimes; ++i)
            if(!(num%primes[i]))
                break;
            else if(num/primes[i] <= primes[i])
                {
                primes[nprimes++]   = num;
                if(num > previous + log)
                    {
                    ++nprint;
                    previous    = num;
                    log     = num / (100 / PERCENTAGE);
                    fout.prime(num);
                    printf( "%u\n", num );
                    }
                break;
                }
        }
    num     = primes[nprimes-1];
    if(num != previous)
        {
        fout.prime(num);
        printf( "%u\n", num );
        ++nprint;
        }
    printf( "nprimes = %d, nprint = %d\n", nprimes, nprint );
    }

void    outfile::open(const char * fname)
    {
    fout   = fopen(fname, "w");

    if(fout == NULL)
        {
        fprintf(stderr, "Can't open output file '%s' for writing.\n",
            fname);
        exit(EXIT_FAILURE);
        }
    }

void    outfile::prime(ulong p)
    {
    int boundary = !(nprimes%4);

    if(nprimes == 0)
        printhead();
    if(nprimes)
        {
        fprintf(fout, ",");
        if(boundary)
            fprintf(fout, "\n");
        }
    if(boundary)
        fprintf(fout, "    ");
    fprintf(fout, "%12lu", p);
    ++nprimes;
    }

void    outfile::printhead()
    {
    char    *type;

    fprintf(fout,

"// machine generated; DO NOT EDIT\n"
"static unsigned short Primes [] = \n"
"    {\n"

    );
    }

void    outfile::printtail()
    {
    fprintf(fout,

"\n"
"    };\n"
"const NPRIMES = %d;\n",

        nprimes
        );
    }
