# Powerful Pointers To Member Functions

**Christopher Skelly**

*Christopher Skelly is President and CEO of Insight Resource Inc., a firm specializing in C++ and OOP training. Insight Resource also developed the KO-PILOT line of help products, which Brit Hume called the “best add-ins ever written”. Christopher speaks regularly at international C++ events, and was the Technical Chairman of the 1992 “C++ in Action” and “CPlusC++” conferences, presented by Boston University. Christopher welcomes email communication at 71005.771@compuserve.com. Insight Resource Inc. can be reached at 914-631-5032.*

---

Pointers, in both C and C++, are handy things, because they give us the right to procrastinate. They allow us to put off until tomorrow, that is, run time, what we would otherwise be forced to do today, at compile time. Such procrastination is often valuable because a run-time engine can sample its actual surroundings, and may be able to make more intelligent choices than those available at compile time. Pointers serve other purposes as well, such as the decoupling of two related objects, which would otherwise be forced to use some scheme of direct containment.

Pointers have changed in C++, even though C++ adheres laudably well to the Spirit of C, preserving the low-level power of a systems engineering language with high-level abstraction capabilities added in.

The most radical new idea C++ brings to pointers is that of a pointer to member, which can be either a pointer to member data or member function. C++’s new pointer types provide both flexibility and localization – they provide the flexibility of run-time decision making, otherwise known as late binding, combined with the localization of class scoping. These pointers are different, in syntax and in some cases implementation, from Standard C pointers. The new pointer types provide the kind of run-time decision making C programmers often implement as an array of pointers to functions. Pointers to members have not yet received the attention they deserve as powerful new tools for implementing complex systems. This article explores the new pointer types, by demonstrating their usefulness and by revealing some aspects of their internal implementation.

## What They Do

It’s easiest to understand what a pointer to member is by seeing what it does. So in this section I present a common programming problem solved by pointers to members, and show what they are along the way. One application for C++’s new pointers is the reduction of Global Name Space Pollution (GNSP). GNSP often occurs as programs grow in size and complexity. More and more names “bubble up” to the global name space, until it becomes difficult to add or change a global symbol without generating an unexpected conflict somewhere in the system. Hiding a function or data pointer inside a class helps avoid this problem.

For example, first consider a simple model, in which an incoming data stream is parsed into separate packets of information. Each packet requires a complex action (call it a “handle”) to properly deal with the packet’s information. Incoming image data from a distant *Voyager* might fit this situation, as might seismographic bundles, complex stock transactions, etc. In this model a class packet defines several member functions to handle the incoming data, with each function designed to handle a different kind of information packet:
```cpp
class packet
   {
public:
   // public interface will go here

private:
   void handletype1(void)
      { /* handle type 1 packet */ }
   void handletype2(void)
      { /* handle type 2 packet */ }
   void handletype3(void)
      { /* handle type 3 packet */ }
   };
```

The following statement creates a pointer to member function of class `packet`:
```cpp
void (packet::*handle)(void);
```

`handle` is the pointer; the member function receives no arguments and returns nothing. This syntax already differs from Standard C, though only one new idea has been introduced, the scope resolving `packet::` construct just to the left of `*handle`. The new construct declares that `handle` is not just an ordinary pointer to function. Instead, `handle` must point at a member function of class `packet`. In other words, the possible values for handle are “localized” to the scope of packet.

However, the preceding example does not completely eliminate the inconvenience of handling global variables. In the declaration above, `handle` is itself a global variable. It’s somewhat contradictory in spirit to have a global variable pointing at a class-scoped function. Therefore, the next step is to place `handle` inside the class declaration, which will reduce its scope to that of `packet`. Users of the class can then “handle” incoming data without concerning themselves with the more complex class internals. Further, making `handle` internal allows new information types to be added to the system with little change required of class clients. Adding a general constructor, class packet now looks like this:
```cpp
class packet
   {
public:
   // handle should be in the public interface
   void (packet::*handle)(void);

    // simple type arg governs construction for the moment
   packet(int type);

private:
   void handletype1(void)
      { /* handle type 1 packet */ }
   void handletype2(void)
      { /* handle type 2 packet */ }
   void handletype3(void)
      { /* handle type 3 packet */ }
   };

// packet constructor sets the handle pointer
 packet::packet(int type)
     {
     switch (type)
         {
     case 1:
         handle =
            &packet::handletype1;
            break;

     case 2:
         handle =
            &packet::handletype2;
            break;

     case 3:
         handle =
            &packet::handletype3;
            break;

         }
     }
```

The constructor above uses a very simple type field to determine which kind of handling is required. A much more elaborate run-time analysis might be performed to make this decision in the real world. In suitable contexts, users of class packet can now handle information packets with syntax like this:
```cpp
packet p1(2);       // create a type 2 packet

(p1.*(p1.handle))();     // handle p1
```

The last statement shows the new `.*` operator at work. This somewhat cumbersome construction provides the appropriate behavior for each packet, yet allows the conceptual simplicity of using `handle` for packets of many different types.

## Not So Simple

The repetition of `p1` in the above statement appears to be awkward at best. Is the repetition strictly necessary? Since `handle` is already a member of `packet`, and `p1` is the invoking object, why not use the simpler syntax shown below?
```cpp
(p1.*handle)();
```

At first glance, this simpler construction might appear to be reasonable. `handle` is a member of the `p1` object. The syntax might mean “execute the `handle` member function of `p1` upon the `p1` object itself”. Why this construction fails reveals an important point in understanding pointers to members in C++.

In C, the `.` and `->` operators both perform a relatively simple task. The `.` operator relates a structure to a member of that structure. For example, `A.B` indicates the `B` member of some struct named `A`. As far as `->` is concerned, if `PA` is a pointer to a struct, then `PA->B` indicates the `B` member of the struct pointed to by `PA`. (Even in C++, `A.F()` implies that `F` is a member function of `A`’s class or superclasses.)

You might suppose that the `.*` and `->*` operators of C++ would represent a parallel connection between an object and its members. But this is not the case. `.` and `->` in C++ have new meanings when used to access member functions, and the `.*` and `->*` operators have a third interpretation, not exactly like that of `.` or `->` in either C or C++.

The `.*` and `->*` operators represent a new pointer idea in C++. They do not imply any membership connection between left and right operands at all – not the actual connection of C or C++ data members, nor the lexical connection of C++ member functions. The `.*` and `->*` operators dereference the pointer to member data, or member function, shown on the right, accessing the appropriate member from the object shown on the left. The pointer to member may or may not actually be a member of that left operand, the so-called *invoking object*.

Hence the extra qualifying (`p1.handle`) must appear as the right-hand operand of `.*`:
```cpp
(p1.*(p1.handle))();
```

The left operand here is the `p1` object, the right operand a pointer to member function of that object’s class or superclasses. Since, except in member functions, members are accessible only through specific objects, we must qualify the whole name of the pointer to member function as the right hand operand of `.*`.

A member function pointer contained in one class can operate on an object of a different class. Though it’s not particulary elegant class design, the next example shows syntactically how a pointer to member function contained in one class might reference member functions of a different class.
```cpp
class A
    {
private:
    void f1();
    void f2();
    friend class B;
    };

class B
    {
private:
    void (A::*pmfi)();
public:

    B(int which = 0)    // constructor sets pmfi
        {
        if (which == 0)
            pmfi = &A::f1;

        else
            pmfi = &A::f2;
        }
    };

A a;
B b(1);
```

Now, in the following statement `a` will dereference the pointer `pmfi` in `b` to invoke the member function `f2()` of class `A`:
```cpp
(a.*(b.pmfi))();
```

A pointer to member function does not have to be a member of any class at all:
```cpp
void (A::*pmfi2)(); // declare a non-class pointer to member

void A::memberfunction() // must be a member to access &A::f1
    {
    pmfi2 = &A::f1;     // set it to A’s f1 address

    (a.*pmfi2)();       // execute the pmfi2 function on a
    }
```

In this example, a dereferences `pmfi2` to invoke the member function `f1()` of class `A`. But what exactly does it mean for an object to be an “invoking” object? Why is it even important which object appears on the left side of `.*` or `->*`? The answers to these questions will become apparent as we examine how pointers to data members and pointers to function members are actually implemented.

## Implementation of Pointers to Members

In C++, pointers to data members are implemented differently from pointers to member functions, and pointers to virtual member functions are different yet again. Understanding how these pointers are implemented will shed light on how they may be expected to behave. Pointers to data members don’t even know what specific address they are going to represent until they are used. Said another way, the same pointer will access a different memory location when dereferenced with different objects. This is very different behavior from that of regular pointers. An ordinary C pointer, once it is set, refers to a specific memory location; it doesn’t matter “who” dereferences it. C++ pointers to data members can access different objects even if their initally assigned value never changes.

### Pointers to Data Members

Pointers to data members in C++ are nothing like their C cousins. A pointer to a data member is implemented as an offset into the class structure. At run time, the system effectively adds this offset to an object’s `this` pointer to determine yet a new (ordinary) pointer which addresses the specific member of that object. With multiple inheritance, the system may add in yet a third factor, known as a delta, to calculate the data member’s actual address. Sometimes the system calls a tiny piece of code called a “thunk” to perform this extra delta addition.

### Pointers to Function Members

Pointers to function members are implemented differently from pointers to data members. That’s because each object contains its own unique data, but all objects of a class share common code for the class member functions. So pointers to member functions store function addresses in a C-like fashion. Member functions determine which object to operate upon by examining the `this` pointer, supplied invisibly by the system to the member function at invocation time. This particular `this` pointer comes from (and points to) the *invoking object*, the object represented to the left of the `.*` and `->*` operators. Now it becomes apparent what role is played by the invoking object; it does not determine which member function gets called, but it determines which object instance that member function operates upon.

### An Example
Before I get into pointers to virtual functions, I want to present a short test program to demonstrate what I’ve described so far. This program ([Listing 1](DEMOPMEM.CPP)) declares several pointers of different types and prints their values after assignment. The program handles regular pointers, pointers to data members and pointers to member functions. Just for fun, the program also tests pointer behavior with static, virtual, and operator functions.

Since the code for `main` is largely repetitive in this test, I use a `SETANDPRINT` macro to print out the pointer values. The macro starts by concatenating an `&` on the member name using the `##` concatenation operator. The program assigns the pointer to the resulting address, then displays both as a string and as a value using the `%p` `printf` format specifier.

Running `pmtest` on my system gives the results shown in Listing 2:
```none
value of pmfv  - &base::memf      - 0311
value of pmfv  - &base::memf2     - 0328
value of pmfv  - &base::doit      - 0747
value of pmfv  - &base::doit2     - 0751
value of pmfv  - &base::doit3     - 075C
value of pmfv  - &base::doit4     - 0767
value of pmfv  - &base::operator~ - 02B5
value of pmfdv - &dev::dmemf      - 0384
value of pmfdv - &dev::dmemf2     - 039B
value of pmfdv - &dev::ddoit      - 0772
value of pmfdv - &dev::doit       - 0747
value of pmfdv - &dev::doit2      - 0751
value of pmfdv - &dev::doit3      - 075C
value of pmfdv - &dev::doit4      - 0767
value of pmfdv - &base::doit      - 0747
value of pfi   - xf               - 0291
value of pdmi  - &base::da        - 0003
value of pdmi  - &base::db        - 0005
```

Notice the difference in results for different types of pointers. `pmfv`, when pointing to a non-virtual member function returning `void`, displays as an address similar to that of `pfi`, the regular C function pointer. `pdmi`, on the other hand, points at a data member of type `int`, and it displays as the small positive integer (an offset) as expected. (Pointers to data members are often implemented to give odd numbers, so the system can distinguish a null pointer from a pointer to a data member at offset `0`.)

Operator member functions are treated no differently than non-operator functions, as you might expect. Operator function syntax allows use of operators to execute functions, but the functions themselves are simply regular member functions. It’s also possible to define non-member operator functions, but they would not be accessible through pointers to members.

### Pointer to Virtual Member Functions

Virtual functions, however, are a whole different story. This test reveals that they are accessed quite differently from non-virtual member functions. The pointer values for all the virtual functions, both inline and non-inline, are the same for both the `base` and `dev` class. `pmfv` has the value `0747` when pointing at `&base::doit`, the same value as `pmfdv` pointing at `&dev::doit`. Yet these pointers actually lead to different functions when dereferenced. How is this possible?

You may recall that each class provides access to its virtual functions through a table of function addresses. This table, often called the *vtable*, is itself accessed through a hidden member, the `vptr`, of each instantiated class object. Only classes with virtual functions have vtables or instantiated objects with `vptrs`. Furthermore, most modern C++ systems create only a single copy of the vtable for each class. Every instantiated object contains its own `vptr` to the class vtable. Retrieving a particular virtual function address in the table should require only a small positive index. Why then don’t the pointers to virtual member functions display as small indices, similar to those of data members?

The answer is that pointers to virtual member functions don’t actually point to member functions at all. Neither do they point or index directly into the class virtual function table. Instead, these pointers reference small pieces of executable code called “thunks”. The thunk’s job is to index into the vtable and jump to the indexed function address. Typically a thunk will consist of only three or four assembly language instructions, with a different thunk being used for each different required index. Thus, as long as the index is the same, the same thunk can access vtables of different classes! That is why both `pmfv` and `pmfdv` point at address `0x0747`. At that location is a thunk which indexes into the class vtable to offset `0`, the offset of `doit` in both class `base` and class `dev`. The thunk terminates by jumping to the function address found at this offset. So `pmfv` actually causes execution to jump to the first function in the base class vtable, while `pmfdv` causes a similar jump to the first virtual function in `dev`’s vtable. Thunks can be more sophisticated than this discussion suggests. Different architectures require different types of thunks, and multiple inheritance throws a whole set of complex machinations into the offset calculations.

## Initializing Pointers to Member Functions

Another way C++ differs from C is in the initialization of pointers to member functions. An ordinary C function name, used without `()` in an expression, evaluates to the address of the function’s code. But to initialize a C++ pointer to member function, you must explicitly supply the `&` in the initializer as in:
```cpp
void (base::*pmfv)(void) = &base::memf;
```

Why is the `&` required in C++, when in C you would typically only name the function to generate a function address? Dr. Stroustrup replied to this very question through email:
> Only `&X::f` yields a pointer to member (see the reference manual under `&`). The reason is that in a member function of `X`, `X::f` means `this->X::f` and I didn’t want `X::f` to mean one thing in one context and something radically different in another.

The idea here is that `X::f` appearing in a member function binds the member name `f` to a particular object, `*this`. We are not allowed to take the address of a bound function name, only that of a generalized class scope member function. To differentiate the meaning of `this->X::f` from the address reference `&X::f`, the `&` is required. Note that this syntactical problem arises only in the presence of member functions, and so C++ must require syntax which we might omit in C. (I note that some versions of AT&T’s cfront allow the `&` to be omitted, which seems to indicate that the presence of `&` may not be critical for proper compiler interpretation.)

## Uses and Alternatives

When might pointers to members be an appropriate mechanism for solving some real world problem? Examples are easy to imagine. An electric circuit might have different current equations depending on how the circuit is damped. A pointer to member function hides the damping details from circuit users. Jim Coplien explores this particular application in the early chapters of his excellent book, “Advanced C++” (the only book I’ve seen which focuses exclusively on the idioms of C++ usage, rather than on language syntax issues).

In the financial arena, pointers to members might switch internally between various data tables, each used for a different sort of stock or option value lookup. A single “lookup” member function pointer might invisibly switch a data member pointer among the internal tables.

Alternates to pointers to members do exist, particularly in the world of C++ idioms. The functor idiom is a powerful alternative, in which functions are “objectified” by inclusion in a particular style of class declaration. Functions, however, merit an article of their own and it’s getting too late to discuss them here.

## Conclusion

Pointers to members are a powerful resource when you need to combine the flexibility of pointers with the localization of class scoping. When an application needs to decide at run time which piece of data to access, or which function to execute next, pointers are appropriate. When the choices are all related to a particular “idea” in the application domain, pointers to members are a better choice than ordinary C pointers. Since C++ provides the logical concept of class scoping, frequently all the relevant members can be organized within a single class abstraction. If you are not deterred by the somewhat unorthodox syntax of member pointers, they can be a great tool in many circumstances.
