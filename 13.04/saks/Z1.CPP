Listing 3 - A rudimentary class a rudimentary class for
complex numbers which recomputes the polar form on demand

// z1.cpp

#include <iostream.h>
#include <iomanip.h>
#include <math.h>

class complex
    {
public:
    complex(double r, double i);
    double real() const;
    double imag() const;
    double rho() const;
    double theta() const;
private:
    double re, im;
    };

inline complex::complex(double r, double i)
    : re(r), im(i)
    {
    }

inline double complex::real() const
    {
    return re;
    }

inline double complex::imag() const
    {
    return im;
    }

inline double complex::rho() const
    {
    return sqrt(re * re + im * im);
    }

inline double complex::theta() const
    {
    return atan2(im, re);
    }

complex operator+(const complex &z1, const complex &z2)
    {
    return complex
        (z1.real() + z2.real(), z1.imag() + z2.imag());
    }


int main()
    {
    complex z1(3, 4);
    cout << '(' << z1.real() << ',' << z1.imag() << ')'
        << endl;
    cout << '(' << z1.rho() << ',' << z1.theta() << ')'
        << endl;
    complex z2(1, 1);
    cout << '(' << z2.real() << ',' << z2.imag() << ')'
        << endl;
    cout << '(' << z2.rho() << ',' << z2.theta() << ')'
        << endl;
    z1 = z1 + z2;
    cout << '(' << z1.real() << ',' << z1.imag() << ')'
        << endl;
    cout << '(' << z1.rho() << ',' << z1.theta() << ')'
        << endl;
    return 0;
    }
