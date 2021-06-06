# How to Leak Memory in C++

**Rodney B. Doe, P.E.**

*Rod Doe has a B.S. in Mechanical Engineering from the University of Washington in Seattle. He is employed as a Senior Software Engineer with Micro Encoder, Inc., in Kirkland, WA. He can be reached via e-mail at rodneyd@microen.com.*

---

Warning: These are untrained professionals at work here. Do not try this at home.

It’s common practice in software engineering to dynamically allocate memory from the operating system for use by a process. C and C++ do not require a process to release this memory when it is no longer needed. When a process neglects to return memory to the operating system, the utility of that memory is lost. Losing the use of memory in this manner is known colloquially as memory leakage.

The purpose of this article is to describe to the practicing software engineer how to achieve memory leakage using the C++ language. The tactics presented here are tried and true and have been proven in actual industrial practice. They are trivially easy to implement. Furthermore, these techniques are portable – they can be applied to any operating system. If used rigorously and diligently, they will provide an adequately debilitating stream of memory leaks for your application.

## Tactic No. 1: Just Don’t Delete It

If you like to keep things simple, then this is the memory leakage tactic for you. To use it, simply omit the applicable `delete` operator on memory that you have dynamically allocated with the `new` operator. Bear in mind, though, that using this technique will brand you as an amateur. Any decent memory error detection tool will flag it. For more insidious memory leakage tactics, read on.

## Tactic No. 2: Use Multiple Return Statements

Remember when some dotty old chalk-covered college professor told you to use only one return statement in a function? Well, clearly, that professor knew nothing about how to leak memory. Using multiple returns actually allows you to exit a function before dynamically-allocated memory is freed. Here is an example:

```cpp
bool AdjustTemperature( HTHING hThing )
{
	if ( !hThing )
		return( false );

	// INSTANTIATE A THING USING A
	// HANDLE
	Thing *pThing = new Thing( hThing );
	if ( !pThing )
		return( false );

	// GET THING'S COLOR AND ADJUST
	// TEMPERATURE ACCORDINGLY
	switch( pThing->Color() )
	{
		case BLUE:
			AdjustTemperature( 5 );
			return( true );

		case RED:
			AdjustTemperature( -5 );
			return( true );

		default:
			break;
	}

	// FOR WHAT COLORS DOES THIS GET
	// EXECUTED?
	delete pThing;

	return( false );
}
```

As you can see in the above code fragment, the programmer saved a few clock cycles by not using a local Boolean variable to keep track of success or failure. He didn’t need a local variable because whenever his code encountered an error it just returned to the caller. This code does return the correct result to its caller. However, if the color of the `Thing` is red or blue, the memory that `pThing` points to never gets deleted. Note that if you must implement memory leakage in legacy code, this technique can also be implemented in good old C by using `malloc` and `free` instead of `new` and `delete`.

## Tactic No. 3: Use Numerous Pointers with the Same Name

C++ allows and encourages a programmer to declare an automatic variable close to the point of usage. So, you can declare an automatic variable that will point to a quantity of allocated memory right at the point of allocation. This capability provides the memory leaker with a unique opportunity. Declare a pointer at the start of a function (in the traditional manner), and declare another pointer of the same name within an inner scope, at the point of instantiation. This will create a fairly confusing memory leak. Here is an example:

```cpp
void ChangeColor( HTHING hThing, int color )
{
	Thing *pThing = NULL;
	if ( IsValid( hThing ) != false )
	{
		Thing *pThing = new Thing( hThing );
		pThing->Color( color );
	}

	// WHICH pThing IS THIS?
	delete pThing;
}
```

In this snippet, two `Thing` pointers called `pThing` are declared. The one inside the if statement points to an instance of a `Thing` object which is never deleted. The `pThing` that does get deleted is null at the time of deletion. C++ guarantees that deleting a null pointer is harmless. Thus, a memory error detection tool may not flag this deletion as an error because it is a valid operation.

A quick glance at the above function makes you think `pThing` gets deleted, so there seems to be no problem. This is a nice technique to slip past code reviewers.

## Tactic No. 4: Use delete on an Array

To use this tactic, allocate an array with the array `new` operator (`new[]`) then release the memory with plain old `delete`, as in:

```cpp
char *pBuf;
pBuf = new char[100];
delete pBuf;  // NOT delete[] pBuf;
```

Probably more than a few readers are suddenly getting a nervous feeling in the pits of their stomachs. If you use the `delete` operator (and not the `delete[]` operator) to release an object that was allocated with `new[]`, the resulting behavior is currently undefined. Even if you get away with this operation now it could bite you later. Or, it may simply leak the memory. Go ahead, put some peril in your life and use it liberally.

## Tactic No. 5: Practice Careless Polymorphism

This tactic takes advantage of the fact that destructors are not inherited. In C++, it is possible and in fact highly desirable to store the address of a derived-class object as a pointer to its base class. You get polymorphic behavior by referencing this derived class object through the base class pointer. The following program instantiates a derived-class object and stores the address as a pointer to its base class:

```cpp
#include <iostream.h>

class Base {
	public:
		Base();
		~Base();
};

class Derived : public Base
{
	protected:
		char *pBuf;

	public:
		Derived();
		~Derived();

};

Base::Base()
{
	cout << "Base::Base()" << endl;
}

Base::~Base()
{
	cout << "Base::~Base()" << endl;
}

Derived::Derived() : Base()
{
	pBuf = new char[100];
	cout << "Derived::Derived()" << endl;
}

Derived::~Derived()
{
	delete[] pBuf;
	cout << "Derived::~Derived()" << endl;
}

void main( void )
{
	Base *pObject = new Derived();
	delete pObject;
}
```

When this program executes, the output will be:

```
Base::Base()
Derived::Derived()
Base::~Base()
```

Note that the destructor for the derived class did not get called. The memory leaker can thus leak the buffer `pBuf` because it was allocated in the `Derived` constructor.

To fix this, simply declare the destructor of the `Base` class as a virtual function, as in:

```cpp
class Base
{
	public:
		Base();
		virtual ~Base();
};
```

As modified, the program output will be:

```
Base::Base()
Derived::Derived()
Derived::~Derived()
Base::~Base()
```

Now the `Derived` class destructor is called, and `Derived::pBuf` is deleted.

## Conclusion

Now you have seen a number of handy tricks of the trade that varsity-level C++ programmers use to implement memory leakage. If you use the tactics described above, you will not have to invent your own. Code reuse is, after all, what C++ is all about. A cautionary note: these memory leak scenarios are made possible by improper use of C++. They are not intrinsic to the language. As such, when your application runs out of memory at a customer site on the other side of the planet, you may not be successful in casting the blame on C++.
