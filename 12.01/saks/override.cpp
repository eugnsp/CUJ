Listing 6 - Selective virtual overriding

#include <iostream.h>

class B
	{
public:
	virtual void f();
	virtual void g();
	virtual void h();
	};

class C : public B
	{
public:
	void f();           // virtual
	};

class D : public C
	{
public:
	void h();           // virtual
	};

void B::f() { cout << "B::f()\n"; }

void B::g() { cout << "B::g()\n"; }

void B::h() { cout << "B::h()\n"; }

void C::f() { cout << "C::f()\n"; }

void D::h() { cout << "D::h()\n"; }

int main()
	{
	C c;
	D d;

	B *pb = &c;         // ok, &c is a C * which is a B *
	pb->f();            // calls C::f()
	pb->g();            // calls B::g()
	pb->h();            // calls B::h()

	C *pc = &d;         // ok, &d is a D * which is a C *
	pc->f();            // calls C::f()
	pc->g();            // calls B::g()
	pc->h();            // calls D::h()

	B &rb = *pc;        // ok, *pc is a C which is a B
	rb.f();             // calls C::f()
	rb.g();             // calls B::g()
	rb.h();             // calls D::h()

	return 0;
	}
