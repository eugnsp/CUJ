# Stepping Up To C++: Mutable Class Members

**Dan Saks**

*Dan Saks is the president of Saks & Associates, which offers consulting and training in C++ and C. He is secretary of the ANSI and ISO C++ committees. Dan is coauthor of C++ Programming Guidelines, and codeveloper of the Plum Hall Validation Suite for C++ (both with Thomas Plum). You can reach him at 393 Leander Dr., Springfield OH, 45504-4906, by phone at (513)324-3601, or electronically at dsaks@wittenberg.edu.*

---

For the past three months, I’ve been listing ways that the programming language described by the C++ Draft Standard (as of Fall, 1994) differs from the language described in the Annotated Reference Manual (ARM [1]). Thus far, I’ve described the *major extensions* (see “[Stepping Up to C++: C++ at CD Registration](../../13.01/saks/saks.md)”, *CUJ*, January 1995). I’ve also described numerous *minor enhancements* (see “[Stepping Up to C++: Minor Enhancements to C++ as of CD Registration](../../13.01/saks/saks.md)” and “[...More Minor Enhancements to C++ as of CD Registration](../../13.02/saks/saks.md)”, *CUJ*, February and March, 1995).

I have a few minor enhancements left to go. This month I present one more such enhancement, mutable class members. Not that mutable members are very important in themselves, but giving the background necessary to explain them raises a lot of interesting design and language issues along the way, so I think it’s worth the time.

Mutable class members provide added support for *logically-const* class objects. Understanding mutable class members requires an understanding of the `const` qualifier, logical vs. physical const-ness, const member functions, and the “casting away” of const. Those of you already familiar with these terms can skip directly to the discussion of lazy evaluation, which presents a problem solved by mutable class members. If you skip too far ahead, you can always skip back.

## The `const` Qualfier

C++ introduced the `const` qualifier as a way to specify read-only objects. When applied to an object, `const` means the program may inspect but not change the value of that object. A C++ translator is therefore free to place const-qualified objects in read-only memory (ROM). In an environment that supports multitasking with shared memory, a translator can place const objects in a sharable, read-only data segment (which is just another flavor of ROM). In some cases, a translator can even optimize a const object out of existence.

For example, given
```cpp
const int MAX = 100;
```
a C++ translator (with the appropriate compiler and linker options) may place `MAX` into ROM. Moreover, if the program never takes the address of, nor binds a reference to `MAX`, a compiler need not generate storage for it. The compiler can simply “optimize away” `MAX` by using its value `(100)` as an immediate operand in generated machine instructions.

C++ erects various barriers to prevent programs from accidentally modifying const objects. For example, a C++ compiler must diagnose every attempt to modify a const-qualified object, such as
```cpp
MAX = 99;     // error
```
or
```cpp
++MAX;        // error
```

Furthermore, the pointer and reference conversion rules won’t let you strip off `const` qualifiers (at least not without casts). For example,
```cpp
int *pi = &MAX;   // error
```
is an error because it attempts to convert a `const int *` (the type of `&MAX`) to an `int *` (the type of `pi`), losing a `const` qualifier in the process. Failure to catch this error would permit an otherwise valid assignment like
```cpp
*pi = 99;
```
to write into a const object. Of course, you can strip a `const` qualifier using a cast, such as
```cpp
int *pi = (int *)&MAX;
```
or the new-style
```cpp
int *pi = const_cast<int *>(&MAX);
```
if you think you know what you’re doing. A cast expression that strips away a `const` qualifier is said to “cast away const”.

When applied to an aggregate, such as an array or `struct`, the `const` qualifier actually percolates down to the elements of the aggregate. That is, the elements of a const array are themselves const objects, as are the members of a const object of a `struct` type. Thus, given
```cpp
const char name[] = "jem";
```
an assignment such as
```cpp
name[0] = 'J';
```
is an error because it tries to alter the value of `name[0]`, which is an object of type `const char`.

## Logically const vs. Physically const

C++ actually supports different degrees of const-ness. Some objects defined const may be *physically* const, that is, residing in ROM and therefore absolutely immutable. Other objects declared const are merely *logically* const. A logically const object can’t be modified in the current context, but it may be modifiable in some other context.

For example, the standard header `<string.h>` declares the `strlen` function as
```c
size_t strlen(const char *s);
```

Inside the body of `strlen`, `*s` designates a logically-const object of type `char`. `strlen` can modify `s`, but it can’t modify `*s` (any character addressed by `s`). On some calls to `strlen`, `s` might point to an actual argument that’s a physically-const object. On other calls, it might point to a non-const object. For example, during the call `strlen(name)`, where `name` is as defined earlier, `s` points to characters that are both logically and physically const. But during the call `strlen(title)`, where `title` is
```c
char title[] = "Fing";
```
`s` is a pointer to logically-const characters that are actually non-const. `strlen` can’t change the characters in `title`, but other parts of the program might.

In general, I recommend using `const` as often as appropriate in declarations. Specifically, I suggest declaring every logically- or physically-const object as such by using the `const` qualifier explicitly. Any purportedly general-purpose library must obey this rule, or it won’t be very useful in ROM-based applications.

For instance, the standard `strcmp` function is declared
```c
int strcmp(const char *s1, const char *s2);
```

`strcmp` compares two strings without altering them. Therefore, inside `strcmp`, both `s1` and `s2` point to logically-const strings, and the declaration says so explicitly. If either `const` were missing, an application could not use this function to compare two const strings. On the other hand, a function such as
```c
char *strcpy(char *s1, const char *s2);
```
can only declare its second parameter as `const`. `strcpy` must reserve the right to alter the string addressed by `s1`, otherwise it can’t do its job of copying `s2` to `s1`.

Even if you don’t write ROM-based applications, you should still use the `const` qualifier generously. Using the `const` qualifier builds more static type information into programs. The compiler can use this information to detect logic errors. As always, the more you tell the compiler about your intent, the more easily it can tell when your program violates that intent. Code that consistently enforces logical constness without casting away const is said to be “const correct”.

## `const` Member Functions

Indeed, writing const-correct classes requires extra effort. For instance, consider the array class template sketched in [Listing 1](ARRAY1.CPP) (this is a templatized version of the `float_array` class I used for my examples in “[Stepping Up to C++: Dynamic Arrays](../../10.11/saks/saks.md)”, *CUJ*, November, 1992). Let’s see what’s required to make it const correct.

[Listing 1](ARRAY1.CPP) includes a non-member function template, `sigma`, that returns the sum of the elements in an `array<T>`. The function declaration:
```cpp
template <class T>
T sigma(array<T> &a)
```
suggests by the absence of any `const` qualifiers that `sigma` might change its actual argument in the course of summing the elements. But logically, `sigma` shouldn't change its argument, and nothing in the function body seems to suggest that it does. Therefore, `sigma`’s parameter should be `const`, as in
```cpp
template <class T>
T sigma(const array<T> &a)
```
so the compiler can enforce the logical const-ness of array `a`.

In effect, the `const` qualifier in the declaration is `sigma`’s promise that it won’t change the actual `array<T>` object referenced by `a`. The compiler then backs that promise by rejecting any statement in `sigma`’s body that tries to change `a`. In particular, since every member of a const object is itself const, the compiler rejects any statement that tries to change a member of `a`. This includes any attempt to pass `a`, or a member of `a`, as a non-const argument to another function.

Recompiling the code after adding the `const` qualifier to `sigma`’s parameter list triggers a number of compile-time errors inside `sigma`. For one, the compiler complains about the call to `a.length()` in
```cpp
for (size_t i = 0; i < a.length(); ++i)
   sum += a[i];
```

The problem is, now that `a` is const, `sigma` can’t call a member function applied to `a` unless it can be sure that the member function won’t alter `a`. How can you tell the compiler that calling `a.length()` won’t alter `a`? By declaring `length` as a *const member function*:
```cpp
size_t length() const;
```

In effect, a const member function promises to treat the object to which it applies as a const object. Once you change `length` to a const member function, `sigma` has no problem calling it.

The keyword `const` after the parameter list actually modifies the object addressed by the implicit `this` parameter. A non-const member function for a class `X` implicity declares `this` as
```cpp
X *const this;
```
That is, `this` is a non-modifiable pointer to a modifiable object. In a const member function, the implicit declaration for `this` is:
```cpp
const X *const this;
```
or equivalently:
```cpp
X const *const this;
```

In other words, `this` is a non-modifiable pointer to a non-modifiable object.

You can apply a const member function to a non-const object. The function simply treats the non-const object as const. However, you cannot apply a non-const member function to a const object, because the non-const member function might try to change the const object.

Adding the `const` qualifier to `sigma`'s parameter list also causes a compile-time error on the call `a[i]` inside `sigma`. Again, the problem is that `array<T>::operator[](size_t)` is a non-const member function. Rather than simply change `operator[]` to a const member function, a better solution is to overload `operator[]` as both const and non-const member functions:
```cpp
const T &operator[](size_t i) const;
T &operator[](size_t i);
```

Hence, the expression `a[i]` invokes the const `operator[]` if `a` is a const array, and invokes the non-const `operator[]` if `a` is non-const. Both forms of `operator[]` return a reference to the selected array element, but the const form returns a reference to a const array element, while the non-const form returns a reference to a non-const element. The const-corrected array class appears in [Listing 2](ARRAY2.CPP) (for more about overloading `operator[]` as both const and non-const member functions, see “[Stepping Up to C++: `operator[]`](../../11.01/saks/saks.md)”, *CUJ*, January, 1993, or Meyers [2]).

## Lazy Evaluation

Without a doubt, the primary benefit of C++ classes is that they support data abstraction. A well-written class defines an abstract type with complete and consistent behavior that you can use with little or no concern for its underlying data representation. It hides the data representation as private data members, and grants restricted access to that data through public member functions.

Most classes have attributes that you can represent in more than one way. For a particular class attribute, you might simply store a value representing the attribute in a data member. Or, you might be able to compute the attribute from other data in the object’s representation.

For example, complex numbers have two common representations:
1. in rectangular form, `(r, i)`, where `r` is the real part and `i` is the imaginary part
2. in polar form, `(rho, theta)`, where `rho` is the magnitude (distance from the origin), and `theta` is the phase angle (typically in radians)

These two forms are related by the following equations:
```none
rho = sqrt(re * re + im * im);
theta = arctan(im / re);
```

If your application for complex numbers uses both forms at one time or another, you might design a complex number class that stores both representations:
```cpp
class complex
   {
public:
   // ...
private:
   double re, im;
   double rho, theta;
   };
```

This design is certainly straightforward, but it has at least two drawbacks:
1. It doubles the storage requirements for every complex number.
2. Every arithmetic operation on complex numbers, such as `+` or `*`, must compute the results twice – once for each form.

As an alternative, your complex class can store just one form, and compute the other form on demand, as shown in [Listing 3](Z1.CPP). The `complex` class in Listing 3 stores only the rectangular form, and recomputes the polar form on demand. For instance, calling `z1.real()` simply returns the value of the private data member `re`, which stores the real part. Calling `z1.theta()` computes the angle using `re` and `im`, the imaginary part.

Note that the four member functions, `real`, `imag`, `rho`, and `theta`, are const member functions. And well they should be. Indeed, none of them changes the value of the complex number. I see no reason to prevent a program from requesting these values of a const complex object.

The problem with computing the polar form on demand is that some applications might calculate the `rho` and `theta` for the same complex numbers over and over again. Granted, the computation is not that complicated, but it uses the `sqrt` and `atan2` functions which are more than just a few instructions on most architectures. If these recalculations prove to be too expensive, a different design for complex numbers might be appropriate.

[Listing 4](Z2.CPP) shows a complex number class that caches the polar form in a dynamically-allocated auxiliary structure. Each complex number has three private data members:
```cpp
class complex
   {
   //...
private:
   double re, im;
   struct polar;
   polar *p;
   };
```

As before, `re` and `im` hold the real and imaginary parts. `p` is a a pointer to an auxiliary structure of type `polar`. `polar` is a forward-declared nested type, defined simply as
```cpp
struct complex::polar
   {
   double rho, theta;
   };
```

That is, it holds the polar representation of a complex number.

By this design, all complex numbers start out by storing only the rectangular form (in `re` and `im`). The constructors always store `0` (a null pointer) into member `p`. Most arithmetic operations, such as `operator+` in Listing 4, use the rectangular forms of the operands, and return a result in rectangular form. Since `operator+` uses a constructor to build the result, that constructor sets the pointer in the result to null.

Like its previous version (in [Listing 3](Z1.CPP)), this complex class never computes the polar form until needed. However, unlike before, this implementation doesn’t just discard the result and calculate again when asked. Rather, it dynamically allocates a `complex::polar` object, and caches the result in that object. If the value of the complex object doesn’t change, the class satisfies repeated requests for the polar coordinates by reading them from the cache, which is much faster than recalculating.

This caching technique is an example of a more general technique known in some circles as “lazy evaluation”. The basic philosophy of lazy evaluation is “Don’t do it unless you have to, and then don’t do it again”. This technique is most useful for any class that has an attribute where
* accessing the attribute’s value incurs a high cost in time or space or both, and
* typical applications request the attribute’s value from relatively few objects.

Meyers [2], Murray [3], and Plum and Saks [4] offer other examples using lazy evaluation (although none uses that term). Meyers uses lazy evaluation to postpone computing the length of string objects. Murray presents a variation on the complex number class. Plum and I sketch a class for tokens (such as in a parser) that caches each token’s hash value during symbol lookup.

## Casting Away const, Again

As in the earlier `complex` class (Listing 3), the four member functions, `real`, `imag`, `rho`, and `theta` of the later class (Listing 4) are also const member functions. Programmers using complex numbers should quite reasonably expect to be able to obtain any of these values from a const complex object, regardless of the implementation. There’s no problem declaring `real` and `imag` in Listing 4
as `const`; they are identical to the corresponding functions in Listing 3.

However, the `rho` and `theta` functions in Listing 4 are a little tricky and require a careful look.

The problem with the `rho` and `theta` functions is that even though they don’t change the value of a `complex` object as seen outside the class, they might change the private data of that object. Specifically, if pointer `p` is null, then `rho` or `theta` must change `p` to point to a newly-allocated `polar` object, using something of the form:
```cpp
if (p == 0)
   p = new polar(..., ...);
```

However, in a const member function of class `complex`, `this` points to a const `complex` object, meaning that `re`, `im` and `p` are themselves const. Thus, the assignment to `p` as written above is an error.

Of course, you can always change `rho` and `theta` to non-const member functions. But then you won’t be able to call `rho` and `theta` for a const `complex` object. Not good. Alternatively, you can leave `rho` and `theta` as const member functions and cast away const from the `complex` object inside each function, as shown in Listing 4.

There are various ways to cast away const inside a const member function. `rho` and `theta` each illustrate a different style. `rho` uses the following approach:
```cpp
complex *This = const_cast<complex *>(this);
This->p = new polar(..., ...);
```

`This` is local variable which points to the same object as `this`. However, `This` is declared without `const`, so `rho` can use it to treat the object as non-const. Copying `this` to `This` loses a `const` qualifer, so the conversion requires a cast to sneak past the compiler. I used the new-style cast. On compilers that don’t yet support new-style casts, use
```cpp
complex *This = (complex *)this;
```

`theta` casts away const a little differently. It simply applies a cast to `p` itself:
```cpp
(polar *&)p = new polar(..., ..);
```

This casts `p` to a reference to a (non-const) pointer to `polar`. Writing the cast as just `(polar *)p` converts `p` to the correct type, but the cast yields an rvalue. An rvalue expression cannot appear on the left of an assignment. Casting to a reference yields an lvalue, which can appear to the left side of an assignment. The reference yields the same result as the slightly longer-winded pointer cast, `*(polar **)&p`.

By the way, Listing 4 uses only two relatively new C++ features: the forward-declaration of nested class `complex::polar` and the new-style cast in `complex::rho`. I found two compilers that will compile and execute Listing 4 as written, namely Borland C++ 4.5 for DOS/Windows, and MetaWare High C++ 3.3 for Extended DOS using the Phar Lap 386 DOS Extender 4.1. If you move the definition for `complex::polar` back inside the definition for `complex`, and change the new-style cast to an old-style cast, most other compilers should accept it.

## At last, Mutable Class Members

If using casts, as I did above, makes you queasy, that’s good. Casts can be pretty dangerous, and you should use them sparingly and with great care. In this case, how can you know that it’s safe to cast away const from a complex number? Functions such as `rho` and `theta` are indeed logically const, but they assume the object is not physically const (it’s not in ROM). If the complex object is really physically const, writing into the object causes undefined behavior.

Mutable class members offer a way to implement logically const operations without casting away const. A mutable member is one that is never const, even if it is a member of an object declared const. For example, in
```cpp
class complex
   {
public:
   // ....
private:
   double re, im;
   struct polar;
   mutable polar *p;
   };
```
Here, `p` is always a non-const object, even in `complex` objects declared const. Thus, const member functions like `rho` and `theta` need not cast away const in order to write to `p`. [Listing 5](Z3.CPP) shows the changes to the `complex` class that result from using a mutable member instead of casts. So far, I’ve found only one compiler that compiles and executes this code – the MetaWare/Phar Lap combination.

Syntactically, `mutable` is a storage class specifier, like `auto`, `register`, or `static`. It can only appear in declarations of class data members. You cannot declare a data member both `mutable` and `const`. For example,
```cpp
class X
   {
   mutable const int *p;  // ok
   mutable int *const q;  // error
   };
```

The declaration for `p` is okay because (ignoring the `mutable` specifier for the moment) `p` is a non-const pointer to a `const int`. Adding `mutable` to the declaration specifies that `p` will always be non-const, even in a const `X` object.

The declaration for `q` is an error because (ignoring the `mutable` specifier for the moment) `q` is a const pointer to a non-const `int`. That is, `q` itself is declared `const`. Adding the `mutable` specifier to the declaration conflicts with the const already applied to `q`, hence the error.

Next month... the remaining minor enhancements.

## References

1. Margaret A. Ellis and Bjarne Stroustrup. *The Annotated C++ Reference Manual*. (Addison-Wesley, 1990).
2. Scott Meyers. *Effective C++.* (Addison-Wesley, 1992).
3. Robert B. Murray. *C++ Strategies and Tactics.* (Addison-Wesley, 1993).
4. Thomas Plum and Dan Saks. *C++ Programming Guidelines.* (Plum Hall, 1991).
