// pmtest.c - display information about
//        pointers to members and member functions
#include <stdio.h>  // for printf prototype
#include <iostream.h>

// The SETANDPRINT macro concatenates an & on the member
// name using the ## concatenation pre-processor operator
// The # stringizing op prints p and v as strings
#define SETANDPRINT(p, v) \
p = &##v; printf("value of %-5s - &%-15s - %p\n", #p, #v, p)

#define PRINT(p, st) \
printf("value of %-5s -  %-15s - %p\n", #p, st, p)

class base;         // forward declarations
class dev;

// declare a pointer to base member function returning void
void (base::*pmfv)();

// declare a pointer to dev member function returning void
void (dev::*pmfdv)();

// declare a pointer to base data member of type int
int base::*pdmi;

// declare an ordinary function returning void
void xf() { cout << "xf()\n"; }

// declare a pointer to ordinary function returning void
//   note that no & is used in the initialization
void (*pfi)() = xf;


class base
       {
public:
       // declare several member functions for pmfv to access
       static void statfun();
       virtual void doit()      { cout << "base doit()\n"; }

       // declare some non-inline virtual functions
       virtual void doit2();
       virtual void doit3();
       virtual void doit4();

       void operator~();
       void memf();
       void memf2();

       // declare two data members for pdmi to access
       // make them public just for easy accessibility in main
       int da, db;

       void (*psmfv)();

       base() { da = db = 9; psmfv = statfun; }
       };

void base::statfun()    { cout << "base statfun()\n"; }

void base::operator~() { cout << "base operator~()\n"; }

void base::doit2()  { cout << "base doit3()\n"; }
void base::doit3()  { cout << "base doit3()\n"; }
void base::doit4()  { cout << "base doit4()\n"; }

void base::memf()   { cout << "base memf()\n"; }
void base::memf2()  { cout << "base memf2()\n"; }


class dev : public base
       {
public:
       virtual void ddoit();

       void doit()  { cout << "dev doit()\n"; }
       void doit2() { cout << "dev doit2()\n"; }

       void doit3();
       void doit4();

       void dmemf();
       void dmemf2();

       int deva;
       };

void dev::ddoit() { cout << "dev ddoit()\n"; }

void dev::doit3() { cout << "dev doit3()\n"; }
void dev::doit4() { cout << "dev doit4()\n"; }

void dev::dmemf()   { cout << "dev dmemf()\n"; }
void dev::dmemf2()  { cout << "dev dmemf2()\n"; }

void main()
       {
       base b;
       dev d;

       // set pointer to member function to address
       // of memf function.  Use the set and print macro to
       // set the pointer to the member and then 
       // display the pointer with %p.

       SETANDPRINT(pmfv, base::memf);

       SETANDPRINT(pmfv, base::memf2);

       SETANDPRINT(pmfv, base::doit);

       SETANDPRINT(pmfv, base::doit2);

       SETANDPRINT(pmfv, base::doit3);

       SETANDPRINT(pmfv, base::doit4);

       SETANDPRINT(pmfv, base::operator~);

       SETANDPRINT(pmfdv, dev::dmemf);

       SETANDPRINT(pmfdv, dev::dmemf2);

       SETANDPRINT(pmfdv, dev::ddoit);

       SETANDPRINT(pmfdv, dev::doit);

       SETANDPRINT(pmfdv, dev::doit2);

       SETANDPRINT(pmfdv, dev::doit3);

       SETANDPRINT(pmfdv, dev::doit4);

       SETANDPRINT(pmfdv, base::doit);

       // print the ordinary pointer for comparison
       PRINT(pfi, "xf");

       SETANDPRINT(pdmi, base::da);

       SETANDPRINT(pdmi, base::db);

       // ERROR: try to set pmfv to static member function
       //           address
       // pmfv = &base::statfun;

       // ERROR: try to set pointer to base member function
       //    to a dev member function address
       // pmfv = &dev::doit;

       cout << "\n\n";
       }


