Listing 5 - A rudimentary class for complex numbers using a
mutable member to implement "lazy" evaluation and caching
for polar form

// z3.cpp

#include <iostream.h>
#include <iomanip.h>
#include <math.h>

class complex
    {
public:
    complex(double r, double i);
    complex(const complex &z);
    complex &operator=(const complex &z);
    ~complex();
    double real() const;
    double imag() const;
    double rho() const;
    double theta() const;
private:
    double re, im;
    struct polar;
    mutable polar *p;
    };

// ... same as Listing 4 ...

double complex::rho() const
    {
    if (p == 0)
        p = new polar(sqrt(re*re + im*im), atan2(im, re));
    return p->rho;
    }

double complex::theta() const
    {
    if (p == 0)
        p = new polar(sqrt(re*re + im*im), atan2(im, re));
    return p->theta;
    }

complex operator+(const complex &z1, const complex &z2)
    {
    return complex
        (z1.real() + z2.real(), z1.imag() + z2.imag());
    }

int main()
    {
    // same as Listings 3 and 4
    }
