# Persistent Vector Iterators

**Radoslav Getov**

*Radoslav Getov has been working as software engineer for twelve years, developing CAD/CAM software for electronics design and manufacturing. You can reach him at rado@enpro.bg.*

---

Template class vector has some annoying properties. But you can hide them, and still make use of the good stuff, by deriving another template class from vector.

Template class `vector` is one of the template container classes from the STL (Standard Template Library). Some of its most important characteristics are:
* It supplies random-access iterators.
* It takes amortized constant time for insertion and deletion of an element at the end of the container.
* It takes linear time for insertion and deletion of an element at the middle of container.
* It performs occasional memory reallocations when a member function must grow the controlled sequence.
* It performs no reallocation in order to free unused memory.
* It invalidating all iterators on reallocation.

I find that the last two "features" are a source of bugs, inefficiency, and at least inconvenience. Invalidating the iterators on reallocation must be controlled (by using the member function `resize`), or it can otherwise create dangling iterators. The lack of deallocation on erase functions can cause memory wastage. (Believe it or not, there is no deallocation in any of the `vector` functions at all.)

The following two examples illustrate these problems. First is an example of a dangling iterator:
```cpp
vector<int> v (100);
.....
vector<int>::iterator i = v.begin();
v.push_back (0);
*i = 0;	// BUG
```

The problem is that `push_back` might cause reallocation, which invalidates the iterator `i`. One solution is to reserve enough space (via a call to the member function `reserve`) at the proper place in the code.

Here is an example of memory wastage:
```cpp
vector<int> v (10000);
.....
v.clear();
cout << "Memory usage: "
     << v.capacity() * sizeof (int)
     << " bytes for " << v.size()
     << " elements. " << endl;
```

The output is:
```
Memory usage: 40000 bytes for 0 elements.
```

At first glance, it might appear that these two problems are unrelated. In my opinion, though, the only reason why no reallocations occur on deletion is to avoid invalidating the iterators on these operations. So, if the iterators were valid on reallocation (they are not), it would have been possible to free the memory on deletion, as well as to probably remove the need for such service functions as `capacity` and `reserve`.

Both these problems do not exist in the other STL containers, except for `deque`. This might also be a problem. A program, working with lists, for instance, might not work if you decide to change `list` to `vector`.

This article presents a variation of the STL `vector` container that features iterators that are persistent on insert operations, and that performs memory deallocation on erase operations.

## Template Class `pvector`

[Listing 1](pvector.h), `pvector.h`, contains the source of a template class `pvector` which is publicly derived from the STL template class `std::vector`. My intention was to make it functionally compatible to `vector`, so that it can replace `vector` without need to change any user code. The only differences are:
* Iterators are persistent on memory reallocations.
* Reallocations happen on erase operations, freeing unused memory.

The most important change to the parent class is the definition of nested classes `iterator` and `const_iterator` (see Listing 1). These definitions override and hide the parent’s `iterator` and `const_iterator`, respectively. Each of these iterator classes defines two member objects:
* a pointer to the container that created it (`_itsVect`)
* an index of the element it designates (`_itsInd`).

An iterator accesses elements of the controlled sequence with expressions of the form `(*_itsVect)[_itsInd]`.

Thus, the iterators that were valid at some moment are still valid even if some reallocation has occurred in the mean time. That solves my first task.

The rest of the member functions make sure that both `iterator` and `const_iterator` fulfill the requirements of a random-access iterator.

## Member Functions

Template class `pvector` defines a number of constructors and member functions, to deal with differences in behavior from the base class. Most of these functions do nothing but call the corresponding parent function, adapting the parameters passed and/or the return type as need be. The interesting exceptions, which behave quite differently from the ones defined in `vector`, are the `erase` functions. Here is one of them:
```cpp
iterator erase (iterator it)
    {
    _BaseType::erase(
        it._getBaseIt());
    if (2 * size() < capacity())
        {
        // deallocate some storage
        pvector copy (*this);
        std::swap (copy);
        }
    return it;
    }
```

Here I overcome `vector`’s unwillingness to free memory by using its copy constructor, which (hopefully) allocates only the small amount of memory required to accommodate the data being copied to it. I then swap the copied contents with `this`. This is the solution for my second task.

Note that storage deallocation happens only if the current vector size is less than half as large as the current capacity. This has the effect that the average deallocation count for `N` erase operation is only `log(N)`. For large `N`, the average reallocation count per operation approaches zero, which effectively yields amortized constant time per operation. (By the way, a similar scheme exists for reallocations on insert operations, too).

Note also that some other member functions like `pop_back` and `clear` also benefit from deallocation, to the extent that they call upon `erase`.

The rest of the member functions, inherited from `vector`, do not need redefinitions. Among them are `size`, `capacity`, `empty`, `clear`, `operator[]`, etc.

## Efficiency

Unfortunatelly nothing comes for free. Vector iterators are mere pointers in most STL implementations. This approach is no doubt more efficient (in both time and space) than the more complex iterator classes defined in `pvector`. Let me try to explain what we lose.

As far as the space is concerned, it is apparent that `pvector::iterator` is most probably twice as large as a pointer. This of course matters if you keep iterators as data. The space requirement for these will double.

As far as speed degradation is concerned, this can be checked with some simple measurements. A simple program [`benchPVector.cpp`](benchPVector.cpp) performs the same test using both `vector` and `pvector` – sorting a randomly initialized vector, using the `sort` algorithm. The result may vary widely across different types of hardware, operating system, compiler, STL implementation, etc. But here are the results I obtained using a Pentium-PRO 200, MSVC 5.0 and NT 4.0:
```
Times: pvector <int> : 4.562
        vector <int> : 2.875
               ratio : 1.58678

    pvector <double> : 5.766
     vector <double> : 3.875
              ratio  : 1.488
```

## Conclusion

More things can be done to improve `pvector`. The redundant information stored in the iterators might be used to check their validity. For instance the code can test whether an iterator belongs to the `this` container in a call to `erase`, or whether dereferencing the iterator is okay, etc.

You can make a small and dirty optimization in comparing iterators – compare just the indexes. This speeds up comparisons a little, but of course yields erroneous results ifyou try to compare iterators from different containers.

`pvector` iterators are somewhat slower, compared to `vector` iterators. On the other hand, the persistence of these iterators, as well as the better memory usage in `pvector`, might compensate for this slower performance. The decision is up to you which container to use.

Finally, I observe that STL supplies yet another container template class that has non-persistent iterators, template class `deque`. You can easily derive a container with better properties, the same way as I’ve shown here for `vector`.
