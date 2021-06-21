# Creating a Word Iterator

**James M. Curran**

*James M. Curran is a Programmer/Analyst, Senior for Memorial Sloan-Kettering Cancer Center in New York City. He has worked as a programmer in C and C++ full-time for the past 12 years, and has twice played tourist at ISO C++ committee meetings. He can be reached at JamesCurran@CompuServe.Com or http://www.NJTheater.Com/JamesCurran.*

---

Iterators can do more than just step through data. They can, for example, do a bit of parsing along the way.

One of the interesting effects of the introduction of the Standard Template Library in the C++ Standard is that it makes us reconsider what are collections and what is being collected. For example, the common view of a string is that it is a collection of characters. The Standard C++ library includes an iterator to step through a string one character at a time. However, at a higher level, many applications would consider a string that holds a sentence to be a collection of words. What we need in that case is an iterator to step through a string one word at a time. I designed class `WordIter` for just that purpose.

The first step was to decide what kind of iterator I’d want it to be. The C++ Standard defines several different categories of iterators, based on their powers and limitations. The decision here didn’t take very long. I saw that most of what I wanted to do could be accomplished just by reading the string from beginning to end. Trying to modify the string via the iterator would be more of a hassle than it was worth. So, I decided to disallow assignment (`*Iter` as a modifiable lvalue). In the parlance of the C++ Standard, an iterator with these properties is called an “input iterator”.

This then defines the list of member functions to include:
* `operator*` (as an rvalue only)
* `operator++`
* `operator==`

See [Listing 1](WordIter.h), the header `WordIter.h`.

The member function `operator->` should also be on the list, in principle, but I could find no meaningful way to represent it. It would have to return the address within the container of the string representing the current word. However, no such string exists as the container itself is a single string. I could only return a copy of the current word, packaged as a string, but that is what `operator*` is for. I have not found a case that needs this missing function.

We must also let the compiler know that this is an input iterator. The C++ Standard supplies the template class `iterator`, from which `WordIter` is derived. The base class supplies information useful to various template functions (algorithms). If a library has several implementations of an algorithm, such as a generic one that uses input iterators and an optimized one that needs random-access iterators for example, the basic algorithm can use the iterator category to choose the correct overload. Template class `iterator` also defines some `typedef`s used in the standard algorithms.

The implementation of class `WordIter` is fairly straightforward. Pointers to constant `char`s point to the beginning and end of the current word. Since no resources are allocated, the default destructor and copy constructor will be sufficient. The heavy lifting is done by the private member function `findword`, which steps through the string setting the beginning and ending pointers. (See [Listing 2](WordIter.cpp).) It is in this function that our concept of a “word” is defined. The version of `findword` given here considers any series of non-whitespace character between whitespaces characters to be a word. This definition is useful for my purposes, but you may want to reconsider it for your work. Note, for example, that this version considers punctuation as part of the word.

Two member functions require special note. `operator*` returns a string that is a copy of the current word. It makes use of a little-known constructor of the `string` class that takes a `char*` and a integer length to create a string exactly that size.

`operator==` requires some thought. For an input iterator, its primary purpose is to determine whether an iterator has reached the end of a sequence. So the function must return `true` if one of its operands is the beyond-the-end iterator. Normally, the container provides such an iterator value, with its member function `end`. But for a string, this value is of type `char*`. We need an object of type `WordIter` so that the beginning and ending iterator match.

The convention is to have the default constructor of an iterator equal a universal beyond-the-end value, so that any uninitialized iterator can be used for the purpose, as in the algorithm call:

```cpp
for_each(wi, WordIter(), PrintString);
```

However, I wanted to make sure that a `WordIter` constructed from the pointer returned by `string::end()` would also function correctly.

I decided that, in general, a beyond-the-end iterator would “point” to a byte of zero (as would a string’s beyond-the-end iterator). For the generic one, I had it point to a particular static byte stored as a class private constant member. Then, in the actual operator function, I first check whether the inital character of each word pointed to is the same. This would be `true` if both iterators have reached the end of the sequence, if both iterators point to the same spot in the same string, or if they both point to separate but identical strings. That quickly gets rid of most comparisons that will test `false`. Next, `operator==` does specific checks for the zero byte or if the two start pointers are the same.

I also added a public static `WordIter` named `EOS`, which is equivalent to a beyond-to-end iterator and can be used in its place, as in:

```cpp
while (wi != WordIter::EOS)
        .....
```

This prevents the need to construct a new empty iterator each time through a loop.

Even though `WordPtr` is just an input iterator, the weakest category, it can still be used with a large number of the Standard C++ library algorithms, such as `copy`, `count`, `find`, `for_each`, and `transform`. (See [Listing 3](WordIterTest.cpp), `WordIterTest.cpp`.)

In cases where you really want to modify the structure of the sentence, it’s best to use `WordIter` to copy the words into a more robust container. The Standard C++ library allows you to do this in one line using `copy` and a “back inserter”, as shown toward the end of [Listing 3](WordIterTest.cpp). So there is little need to enhance `WordIter` with the ability to alter the original string.

Note also that, like any Standard C++ library iterator, any change to the container into which an iterator points (in this case, the string holding the full sentence) invalidates the iterator, and any use of the iterator afterwards is undefined behavior. For those using MFC, conversion from using `string`s to `CString`s is a trivial matter, most of which can be done with your text editor’s “replace all” command. The only manual editing needed is in the “create from string” constructor.
