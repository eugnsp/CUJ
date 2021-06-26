Listing 8 - Derived class that doesn't override all pure virtual 
functions is still abstract

class B
	{
public:
	virtual void f();
	virtual void g() = 0;
	};

void B::f() { ... }

class D : public B
	{
public:
	void f();   // virtual
	            // g is still pure virtual
	};

void D::f() { ... }

int main()
	{
	B b;        // error, B is abstract class
      D d;        // error, D is abstract class
	...
	}
