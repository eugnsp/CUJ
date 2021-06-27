# C++ Theory and Practice: `new` and `delete`

**Dan Saks**

*Dan Saks is the president of Saks &Associates, which offers consulting and training in C++ and C. He is secretary of the ANSI and ISO C++ committees. Dan is coauthor of C++ Programming Guidelines, and codeveloper of the Plum Hall Validation Suite for C++ (both with Thomas Plum). You can reach him at 393 Leander Dr., Springfield OH, 45504-4906, by phone at +1-937-324-3601, or electronically at dsaks@wittenberg.edu.*

---

C++ has a more structured approach than C to allocating storage on the fly, but it’s not always less error prone.

C++ provides the `new` and `delete` operators as alternatives to the standard C library functions `malloc` and `free`, respectively. For typical uses, `new` and `delete` are safer than `malloc` and `free`, and possibly more efficient. However, the syntax for `new` expressions is considerably more complicated than the syntax for calling `malloc`, and lurking in that complexity is the potential for some pretty subtle bugs. The same is true (to a lesser degree) for `delete` expressions in comparison with calling free.

This month, I begin a detailed look at `new` expressions, paying particular attention to some of the potential pitfalls. Although I presume you already have experience with `new` and `delete` expressions, I’ll begin somewhere near the beginning. I’ll use this review to clarify some issues that seem to cause confusion even for experienced C++ programmers.

## Allocating Individual Objects

In C, you allocate dynamic memory using the standard library function:

```c
void *malloc(size_t n);
```
(`size_t` is the result type of the `sizeof` operator. Various standard headers define `size_t` as a `typedef` equivalent to either `unsigned int` or `unsigned long int`.) For example,

```c
p = malloc(sizeof(T));
```
allocates storage for a single object of type `T` and stores the address of that object in `p`. `p` presumably has type `T*`.

I say “presumably” because C allows type conversion errors here. `malloc` returns the address of the allocated object as a `void*`. C allows conversion from `void*` to any other pointer type, even without a cast. Thus, the call to `malloc` above compiles in C even if `p` is declared as a pointer to some type other than `T`. That’s an accident waiting to happen.

The stricter type conversion rules of C++ prohibit quiet conversions from `void*` to `T*`. Thus, the previous call to malloc won’t compile in C++ unless you add a cast, as in:

```cpp
p = (T *)malloc(sizeof(T));
```

Rather than make you use a cast (a sign of possible danger) for what should be a perfectly safe operation, C++ provides the `new` operator to do that conversion automatically:
```cpp
p = new T;
```

`new` is a keyword in C++. For type `T`, `new T` is a `new`-expression that allocates dynamic storage for an object of type `T`. The `new`-expression returns the address of the allocated object as a `T*`.

In C, you use the standard function:
```c
void free(void *p);
```
to deallocate (reclaim) storage allocated by `malloc`. In C++, you use the predefined `delete` operator to deallocate storage acquired by `new`. For example, given:
```cpp
p = new T;
```
then the `delete`-expression:
```cpp
delete p;
```
deallocates a single object of type `T`.

As with `free`, deleting a null pointer has no effect. You need not check that a pointer is not null before deleting it.

## The Free Store vs. the Heap

Whereas `malloc` and `free` manage memory from a region known as the heap, `new` and `delete` manage memory from a logically separate region called the free store. The free store and heap can coexist in a program. This coexistence lets you create C++ programs (which use `new` and `delete`) by linking with C components (which use `malloc` and `free`).

In many C++ implementations, the default implementations of the free store and the heap are the same. That is, whether you call `malloc` or use a `new`-expression, the storage for the object comes from the same memory region according to the same allocation strategy. But the draft C++ Standard makes no promise that this is so. Therefore, you should program as if the free store and heap are distinct. (That is what I meant when I said the free store is logically separate from the heap.) In particular,
* if you allocate memory using `new`, you should `delete` it (rather than `free` it), and
* if you allocate memory using `malloc`, you should `free` it (rather than `delete` it).

Many C++ programmers use the term “heap” when referring to either the heap or the free store. I suspect they are unaware of the distinction. In practice, I originally didn’t think this difference would cause any real confusion, but I can see now that it might when dealing with mixed C/C++ applications. At the risk of being too pedantic, I recommend that you use the term “free store” rather than “heap” when discussing `new` and `delete`.

## Construction and Destruction

Both `malloc` and `new` allocate storage, but only `new` applies a constructor to the allocated storage. That is, if `T` is a class type
```cpp
p = new T;
```
allocates storage for a `T` object and applies `T`’s default constructor to that object. In contrast,
```c
p = (T *)malloc(sizeof(T));
```
merely allocates storage and leaves the object uninitialized.

If `T` is a class type with no default constructor (C++ generates a default constructor for `T` only if `T` has no explicitly-declared constructors), or if `T`’s default constructor is inaccessible (because it’s private or protected), then
```cpp
p = new T;
```
is a compile-time error. If you still want to dynamically allocate a `T` object, you must specify arguments for some other constructor, using a `new`-expression of the form:
```cpp
new T(argument-list);
```

C++ uses the usual argument matching rules used in function overload resolution to select a constructor with parameters that best match the argument list. If there’s no unique best match, or if the best matching constructor is inaccessible, the compiler rejects the `new`-expression.

For example, if class `T` has constructors
```cpp
T();
T(int i, double d);
```
then
```cpp
p = new T(1, 4.5);
```
uses `T(int, double)` to initialize the `T` object, and
```cpp
p = new T(0);
```
is an error.

Just as a `new`-expression invokes a constructor, a `delete`-expression invokes a destructor. Thus,
```cpp
delete p;
```
applies `T`’s destructor to the object addressed by p before deallocating it. In contrast,
```c
free(p);
```
simply deallocates the storage.

Strictly speaking, not all types have constructors and destructors. In particular, scalar types (arithmetic, enumeration, or pointer types) do not have constructors and destructors. However, it’s easier to describe the behavior of construction and destruction if we presume that they do. Part of the presumption is that each of these types has a default constructor and a destructor that are trivial – they do nothing.  Thus, a compiler can “optimize-away” (avoid generating code for) calls to these trivial functions.

For example, if `pi` has type `int*`, then
```cpp
pi = new int;
```
allocates space for an `int` object an applies `int`’s default constructor to that object. But that default constructor is trivial, so the compiler generates no initialization code, leaving `*pi` uninitialized.

You can even supply a single “constructor” argument when allocating an object of scalar type `T`. The argument must have type `T`, or a type that converts to `T`. For example,
```cpp
pi = new int(100);
```
allocates an `int` object and initializes it with the value `100`. In a sense, each scalar type has a copy constructor that initializes an object of that type by simple assignment. That is, the previous `new` expression has essentially the same effect as:
```cpp
pi = new int;
*pi = 100;
```

Actually, scalar types aren’t the only types with trivial constructors and destructors. POD types have them also. A POD type is essentially a type you can write in C. (I’m not making this up. The draft C++ Standard actually uses the term POD, pronounced as “pod”, and short for “plain old data”.) More precisely, a POD type is:
* a scalar type
* a `class`, `struct`, or `union`, with no user-defined constructor or destructor, no base classes, no virtual functions, and all of whose data members are public members with POD types
* an array with elements of POD type

The constructor argument list in a `new`-expression can be empty. For example,
```cpp
p = new T();
```
is valid. For a non-POD type `T`, it means the same as
```cpp
p = new T;
```

That is, it invokes `T`’s default constructor. However, for POD types, there’s now a subtle difference between these two forms of `new`-expression.

In particular, if `T` is a POD type, then
```cpp
p = new T;
```
leaves the allocated object uninitialized, while
```c
p = new T();
```
initializes the object with zero (as if it were statically-allocated).

This distinction between `new T` and `new T()` was part of a larger change wrought by the C++ standards committee a couple of years ago. The change affected the meaning of all initializers of the form `T()`. (See “[Stepping Up to C++: Other Assorted Changes, Part 3](../../13.09/saks/saks.md)”, *CUJ*, September 1995.) Not all compilers make this distinction yet. Some still treat `new T()` the same as `new T` for all types.

## `operator new` and `operator delete`

A `new`-expression actually allocates memory by calling a function called `operator new`. Each C++ environment provides a default implementation for a global function `operator new`, declared as
```cpp
void *operator new(size_t n)
    throw(std::bad_alloc);
```

As with `malloc`, the argument to `operator new` is the size (in bytes) of the requested storage.

Whereas `malloc` returns a null pointer if it can’t allocate the requested storage, `new` throws an exception if the allocation request fails. The `throw` specification:
```cpp
throw(std::bad_alloc)
```
at the end of the function declaration indicates that `operator new` will throw only exceptions of type `std::bad_alloc`. `std` is the namespace that houses the standard library components, and `bad_alloc` is one of the standard exception types.

Thus, an expression such as
```cpp
p = new T;
```
translates more-or-less into something like
```cpp
p = (T *)operator new(sizeof(T));
p->T();
```

The first statement acquires storage for a `T` object by calling `operator new`, and converts the address of that storage from type `void*` to type `T*`. The second applies the default constructor to the object. (That second statement – an explicit constructor call – is not something you can actually write in C++.)

 A `delete`-expression deallocates memory by calling a function called `operator delete`. Each C++ environment provides a default implementation for a global function `operator delete`, declared as:
```cpp
void operator delete(void *p);
```

If `p` has type `T*`, then an expression such as
```cpp
delete p;
```
translates into something resembling

```cpp
if (p != NULL)
{
    p->~T();
    operator delete(p);
}
```

The explicit destructor call (the first statement after the opening brace) is something you can write in C++.

## Allocating Arrays

In C, you allocate an array of n objects of type `T` using a `malloc` call of the form:
```c
p = malloc(n * sizeof(T));
```

In C++, you use a `new`-expression of the form:
```cpp
p = new T[n];
```

This form is called an array `new`-expression. The array `new`-expression above returns a `T*` that points to the first element of the allocated array.

The expression inside the brackets can be any integer-valued expression, not just a compile-time constant expression. For example,
```cpp
char *t = new char[strlen(s) + 1];
```
is a valid array `new`-expression that allocates an array of characters large enough to hold a copy of the null-terminated character string `s`. (The `+ 1` accounts for the null character at the end.)

An array `new`-expression allocates memory by calling a function called `operator new[]`. Each C++ environment provides a default implementation for a global function `operator new[]`, declared as
```cpp
void *operator new[](size_t n)
    throw(std::bad_alloc);
```

The argument to `operator new[]` is the size (in bytes) of the requested storage, not the number of array elements.

`operator new[]` is an invention of the C++ standards committee, now available in some (but not all) C++ compilers. Previously, array `new`-expressions invoked `operator new` (the same function as for non-arrays).

An array `new`-expression such as
```cpp
p = new T[n];
```
applies `T`’s default constructor to each element of the allocated array, in order of increasing address. Thus, the previous expression translates into something like
```cpp
p = (T *)operator new[](n * sizeof(T));
for (size_t i = 0; i < n; ++n)
    p[i].T();
```

I believe most compilers don’t generate the loop at the point of the `new`-expression. Rather, they call a run-time support function that performs the loop. Again, the explicit constructor call `p[i].T()` appearing in the loop body is not something you can actually write in C++. Nonetheless, the compiler manages to apply `T`’s default constructor to each `p[i]`.

## Deallocating Arrays

C++ provides an alternative form of `delete`-expression called array `delete` for deallocating arrays. For example,
```cpp
delete [] p;
```
is an array `delete`-expression that deallocates the array of `n` objects of type `T` addressed by `p`.

The form of a `delete` expression (array or non-array) must match the type (at run time) of the object that it’s deallocating. For example, suppose `T` is a non-array type and `p` is an object of type `T*`. If `p` points to an object acquired by
```cpp
p = new T;
```
then the corresponding `delete`-expression must have the form
```cpp
delete p;
```

On the other hand, if `p` points to an array of `T` acquired by
```cpp
p = new T[n];
```
then the corresponding `delete`-expression must have the form
```cpp
delete [] p;
```

An array `delete`-expression also applies a destructor to each element in the array, in order of descending address. (As always, destruction is in reverse order of construction.) The `delete`-expression then deallocates memory by calling a function called `operator delete[]`. Each C++ environment provides a default implementation for a global function `operator delete[]`, declared as:
```cpp
void operator delete[](void *p);
```

Thus, if `p` has type `T*`, then an array `delete`-expression such as
```cpp
delete [] p;
```
translates into something resembling
```c
if (p != NULL)
{
    size_t i = n;
    while (i-- > 0)
        p[i].~T();
    operator delete[](p);
}
```

Here, `n` is the number of elements in the array. I explain how the program finds `n` a bit later. The expression `p[i].~T()` in the loop body is an explicit destructor call applied the to `i`-th array element.

Again, the form of the `delete`-expression must correspond to the actual type of the object that it’s deallocating. It is not necessary that the form of the `delete`-expression match the form of the `new`-expression that allocated the object. For example, suppose `T` is a class type, and `A` is an array type defined by
```cpp
typedef T A[10];
```

If `p` has type `T*`, then
```cpp
p = new A;
```
is a valid `new`-expression that allocates an array of `10` objects of type `T`, and places the address of the first element in `p`. The requisite `delete`-expression is the array `delete`:
```cpp
delete [] p;
```
because `p` points to (the first element of) an array. You should use an array `delete` even if the array has only one element.

What happens if you mix them up? That is, what happens if you attempt to deallocate an array using non-array `delete`, or vice versa? The draft C++ standard says the behavior is undefined. That is, something bad may happen, maybe now or maybe later. More precisely, a program that uses the wrong form of `delete`-expression has committed an error which neither the compiler nor the runtime system is obligated to diagnose. The program might terminate abruptly, or limp along producing erroneous results. It might even get away seemingly unscathed.

Since a pointer can point to either a single object or the first element in an array, compilers cannot in general tell if you’ve used the wrong form of `delete`. It’s your responsibility to select the right form for each `delete`-expression, with little or no help from your compiler(s). To many programmers first learning C++, this seems contrary to the prevailing spirit of C++, which places greater emphasis on compile-time type checking than does C. In C, you cannot choose the wrong form for calling `free` because there is only one, which you call whether you are disposing of a single object or an entire array.

The reason that C++ imposes this burden on programmers is that it enables compilers to eliminate some run-time overhead when allocating individual objects. The design of C++ attempts to balance safety with performance, and in this case, it opts for performance.

General-purpose memory managers, such as `malloc` and `free`, must handle objects of different sizes. Thus, when `malloc` allocates a chunk of memory, it must stash the size of the chunk somewhere where `free` can later find it. In a typical implementation, each call to `malloc` actually grabs a few more bytes than were requested, and stores the size of the chunk in those extra bytes. A later call to `free` uses that size to determine how much to deallocate.

Typical default implementations for `operator new` and `operator delete` behave much the same. In fact, some (possibly many) default implementations for `operator new` call `malloc` to do the bulk of the work. Ditto for `operator delete` and `free`. However, with modest effort, you can replace a default implementation for `operator new` and `operator delete` with specialized implementations that avoid the need to store object sizes at run time.

It’s relatively easy to avoid storing object sizes when allocating and deallocating individual objects, but generally much harder when allocating arrays. By separating the `delete`-expressions into two forms, C++ makes it easier for you to write higher-performance memory managers that deal only with individual objects, without forcing you to conform to the more difficult problems of managing arrays.

## More to Come

Well, that covers most of the basics of `new` and `delete`. Stay tuned for more on this topic.
