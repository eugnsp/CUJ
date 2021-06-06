/*
 * random.cpp
 * Source code file for the implementation of a random number generator class.
 * This class is based on the algorithm defined by Z(i) = AZ(i-1)modM.
 * Where M is the prime number 2^31-1 = 2,147,483,647 and A is the value
 * 62089911.
 * A statistically "correct" values of A is used, based on the paper "An
 * Exhaustive Analysis of Mulitplicative Congruential Random Number Generators
 * with Modulus 2^31-1", Fishman and Moore, SIAM J. Sci. Stat. Comput., Vol. 7,
 * No. 1, January 1986, 24-45
 * Finally, the multiple random number stream feature has been added utilizing
 * an adaptation of the algorithm published in "Implementing a portable FORTRAN
 * Uniform (0,1) Generator", Marse and Roberts, Simulation, October 1993, 135 -
 * 139.
 */
#include "random.h"
#include <math.h>

#define RANDOM_DEBUG 0 // set non-zero to compile main
#if RANDOM_DEBUG
    #include <fstream.h>
    #include <stdlib.h>
#endif
// Static variables
int RandomGenerator::stream_count = 0;
long int RandomGenerator::base_seed;

// Constants
const double MODULUS = 2147483647.0L;    // 2^31 - 1 =  2,147,483,647
const double A_100000 = 241748845.0L;    // A^100000 mod MODULUS
const double A = 62089911.0L; // From reference

RandomGenerator::RandomGenerator(void)
{ // Constructor
    if (stream_count++ != 0) // increment stream counter for next constructor
    { // This is not the first stream, a new base seed must be calculated
        /*
         * The actual algorithm for this calculation is:
         * Z(100000) = (Z(0)*A^100000)modm. Because of obvious overflow problems,
         * A^100000modm was precomputed to be 241748845. The math behind this was
         * (A*B)modm = ((Amodm)*(Bmodm))modm. The new seed is then computed as
         * Z(100000) = (241748845*Z(0))modm
         */
        base_seed = (long int)fmod((double)base_seed * A_100000, MODULUS);
    }

    // Set the first seed value for the stream
    my_start_seed = current_seed = base_seed;
}

double RandomGenerator::Rand(void)
{
    current_seed = (long int)fmod((double)current_seed * A, MODULUS);
    return (double)current_seed / MODULUS;
}


#if RANDOM_DEBUG
int main(int /*argc*/, char */*argv*/[], char */*env*/[])
{
    // Initialize the static variable
    RandomGenerator::ResetStreamCount();
    RandomGenerator::SelectSeed(1000);

    // Create random number generators
    RandomGenerator X;
    UniformRandomGenerator U;
    ExponentialRandomGenerator E;
    TriangleRandomGenerator T;

    // Check out uniform
    double x = X.Rand(); // Get first number
    double y;
    for (long int count = 0; count < MODULUS; count++)
    { // If full cycle will never have a repeat!
        y = X.Rand();
        if (x == y)
            cout << "Full Cycle Test Failed! Count = " << count << endl;
        if (!(count % 1000))
            cout << "Count: " << count << endl;
        if (count == 100000L - 1) // Is this Uniform?
        {
            if (y == U.Rand())
                cout << "Uniform Test Passed!" << endl;
        }
        else if (count == 200000L - 1) // Is this exponential?
        {
            if (-1.0 * log(y) == E.Rand())
                cout << "Exponential Test Passed!" << endl;
        }
        else if (count == 300000L - 1) // Is this triangle?
        {
            if (y > 0.5)
            {
                if (1.0 - sqrt(0.5 * (1.0 - y)) == T.Rand())
                    cout << "Triangle Test Passed!" << endl;
            }
            else
            {
                if (sqrt(0.5 * y) == T.Rand())
                    cout << "Triangle Test Passed!" << endl;
            }
        }
    }
    cout << "Full Cycle Test Passed!" << endl;

    // Cleanup and good night Irene
    exit(0);
    return 0;
}
#endif
