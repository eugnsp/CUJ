Listing 4 -  A rudimentary class for complex numbers using
"lazy" evaluation and caching for polar form

// z2.cpp

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
    polar *p;
    };

struct complex::polar
    {
    polar(double r, double t);
    double rho, theta;
    };

inline complex::polar::polar(double r, double t)
    : rho(r), theta(t)
    {
    }

inline complex::complex(double r, double i)
    : re(r), im(i), p(0)
    {
    }

inline complex::complex(const complex &z)
    : re(z.re), im(z.im), p(0)
    {
    }

inline complex &complex::operator=(const complex &z)
    {
    re = z.re;
    im = z.im;
    p = 0;
    return *this;
    }

inline complex::~complex()
    {
    delete p;
    }

inline double complex::real() const
    {
    return re;
    }

inline double complex::imag() const
    {
    return im;
    }

double complex::rho() const
    {
    if (p == 0)
        {
        complex *This = const_cast<complex *>(this);
        This->p =
            new polar(sqrt(re*re + im*im), atan2(im, re));
        }
    return p->rho;
    }

double complex::theta() const
    {
    if (p == 0)
        (polar *&)p =
             new polar(sqrt(re*re + im*im), atan2(im, re));
    return p->theta;
    }

complex operator+(const complex &z1, const complex &z2)
    {
    return complex
        (z1.real() + z2.real(), z1.imag() + z2.imag());
    }

int main()
    {
    // same as Listing 3
    }
