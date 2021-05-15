///////////////// benchPVector.cpp ///////////////////
//
// Compare the times of pvector<> and std::vector<>.
// The tests perform certain ammount of std::sort() 
// on vectors.
//
// Author: Radoslav Getov
//
//////////////////////////////////////////////////////


#include "pvector.h"     // for pvector<>

#include <iostream>      // for cout, etc
#include <iomanip>       // for endl
#include <algorithm>     // for generate(), sort()

#include <time.h>        // for clock()

using namespace std;

const size_t SIZE  = 1000;  // of vectors
const int    LOOPS = 5000; // in the test

// -------------- DO_MEASURE --------
#define DO_MEASURE                                   \
                                                     \
   srand (0);                                        \
   clock_t start = clock();                          \
   for (int j = 0; j < LOOPS; j++)                   \
      {                                              \
      std::generate (p.begin(), p.end(), rand);      \
      std::sort (p.begin(), p.end());                \
      }                                              \
   return (double)(clock() - start) / CLOCKS_PER_SEC; 

// --------- measure pvector <int> ----------
static double measurePvectorI()
{
pvector<int> p (SIZE);
DO_MEASURE                               
}

// --------- measure vector <int> -----------
static double measureVectorI()
{
std::vector<int> p (SIZE);
DO_MEASURE                               
}

// --------- measure pvector <double> -------
static double measurePvectorD()
{
pvector<double> p (SIZE);
DO_MEASURE                               
}

// --------- measure  vector <double> -------
static double measureVectorD()
{
std::vector<double> p (SIZE);
DO_MEASURE                               
}

// -------- compare times --------
void benchPVector()
{
double t_v_i   = measureVectorI  (),
       t_p_i   = measurePvectorI (),
       ratio_i = t_p_i / t_v_i;

cout << "Times: pvector <int> : " << t_p_i   << endl
     << "        vector <int> : " << t_v_i   << endl
     << "               ratio : " << ratio_i << endl
     << endl;


double t_p_d = measurePvectorD (),
       t_v_d = measureVectorD  (),
       ratio_d = t_p_d / t_v_d;

cout << "    pvector <double> : " << t_p_d   << endl
     << "     vector <double> : " << t_v_d   << endl
     << "              ratio  : " << ratio_d << endl
     << endl;
}


