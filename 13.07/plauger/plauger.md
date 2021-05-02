# Standard C/C++: The Header `<string>`

**P.J. Plauger**

*P.J. Plauger is senior editor of C/C++ Users Journal. He is Convener of the ISO C standards committee, WG14, and active on the C++ committee, WG21. His latest books are **The Draft Standard C++ Library**, and **Programming on Purpose** (three volumes), all published by Prentice-Hall. You can reach him at pjp@plauger.com.*

---

## Introduction

One of the great strengths of C, from its earliest days, has been its ability to manipulate sequences of characters. The null-terminated sequence, or string, even has a bit of language support. You can write a string literal, such as `"abc"`, and the translator will construct for you a non-modifiable array of characters containing the sequence you specify terminated by a null character.

That’s about all the support you can expect from the C language proper. The Standard C library adds a number of functions, all declared in `<string.h>`, for manipulating strings and other kinds of character sequences. Amendment 1 to the C Standard adds `wchar_t` analogs to these functions, all declared in `<wchar.h>`. But you have next to no help in managing storage for these sequences. C programs abound with oversize arrays to hold character sequences of worst-case length. Or they contain ornate logic to allocate and free storage, copy strings about, and get those terminating null characters where they belong.

Little wonder, then, that writing string classes is one of the more popular indoor sports among C++ programmers. Encapsulation is an ideal tool for managing storage for strings, for hiding implementation details, and for enforcing semantic restrictions. A string class is a natural addition to any C++ library. In fact, essentially every implementation of C++ offers one, in some from or another.

And therein lies a problem. Many string classes exist in prior art, but they are many *different* string classes. Committees X3J16 and WG21 faced a double challenge. They recognized a widespread demand for a standard string class, but couldn’t identify an existing one to standardize on. To “codify existing practice” they had to be simultaneously inventive.

## Class `string` and Beyond

The header `<string>` is thus an invention of the Committee, but one at least originally based heavily on prior art. In its first incarnation, it endeavored to define a class `string`, and a mess of supporting functions, that programmers should find compelling enough to use widely. That suggests reasonable efficiency in execution time and storage consumption, as well as an adequately rich assortment of operations. I believe those goals were probably well enough met by the original class `string`, as of about a year ago. Since then, however, it has undergone a series of changes.

The first change was the addition of a wide-character analog to class `string`. Following the lead of Amendment 1 to the C Standard, the Committee endeavored to provide parallel capability based on `wchar_t` to practically everything in the draft Standard C++ library based on `char`. Thus was born class `wstring`, which survived for most of a year.

Having two closely parallel classes is distasteful, particularly in a language that supports templates. You’d think that the Committee would have considered a string template early on, rather than indulge in a bolt-on like class `wstring`. In fact, templates were a topic of frequent discussion, for use in the library, since they first went in the C++ language four years ago. But repeatedly, people noted limitations of templates that interfered with the expressiveness or efficiency that people had come to demand of string classes. I won’t go into detail here, because the arguments are now of interest only to historians. Suffice it to say that the Committee considered and rejected templates for strings (and iostreams, and complex arithmetic) several times before finally adopting that approach widely throughout the library.

It’s hard to say exactly what caused the change of heart. One factor was the growing experience Committee members accumulated as template implementations became more widespread. Another was small but steady improvements in how templates were translated to executable code. In the end, however, most of the early objections were not so much met and overcome as they were given less weight. People simply changed their style of programming, and their expectations, as they shifted to writing template-based C++ code.

Thus, the Committee eventually replaced class `string` with a template class called `basic_string`. One instantiation, for elements of type `char`, reproduces class `string` and related functions. Another instantiation, for elements of type `wchar_t`, reproduces class `wstring` and related functions. This approach closely resembles the templates introduced for the iostreams classes, but is independent.

As with iostreams, the basic *functionality* of class `string` did not change with the introduction of templates. A program written to use the original class `string` (or `wstring`) should remain essentially unchanged. The implementation is now a bit harder to read, and somewhat messier. Worse, the header `<string>` is now *much* larger, and it has to be included in practically every Standard C++ program you write.

The header is large because it must now contain the definitions for all the numerous template member functions that class `basic_string` has accumulated over the years. (If there is a portable way to avoid placing all those template definitions in a header, I’ve yet to have it explained to me in terms I can understand.)

There are lots of member functions because this class has become all things to all people. It began as essentially the union of all the string classes in widespread use. It acquired more functions to achieve a kind of completeness of functionality. It picked up a bit more machinery as a side effect of being templatized. Most recently, it acquired another slew of member functions to work better with the Standard Template Library – another recent and ambitious addition to the draft Standard C++ library.

Finally, header `<string>` is largely unavoidable because the iostreams machinery has one or two declarations that require a definition of class `string` to work properly. The net effect of all this templatization and cross referencing is an explosion in the size of the standard headers. The classic minimum program:
```cpp
#include <iostream>

int main()
   {
   cout << "hello, world" << endl;
   }
```
now drags in *several thousand* lines of headers. A significant chunk of this total is the definition of template `basic_string`. (Yes, I’ve raised this issue with the Committee, but so far with only limited success.)

## Using `<string>`

You include the header `<string>` to make use of the template `basic_string`. The best known instantiation of this template is known by the type definition `string`. It is the template specialized for string elements of type `char`. I confine my descriptions this month to just that common instantiation, to avoid confusing detail.

Objects of this class let you manipulate in-memory character sequences that vary dynamically in length. The `string` object allocates and frees storage as needed to accommodate changes in length of the controlled sequence.

Class `string` has lots of member functions. The header also defines lots of additional functions that have one or more `string` parameters. But many of these come in sets of three or more. The idea is to provide a “fat” interface. Almost anywhere you can write a `string` argument, you should also be able to write any of:
* a pointer to a character sequence, followed by a sequence length argument
* a pointer to a null-terminated character string
* a repetition count, followed by a character to be repeated
* a range of characters delimited by two *iterators* (in the style of the Standard Template Library)

The last option is the most recent addition, as I indicated earlier. Typically, it takes the form of a *member template function*, parametrized on the type of the iterators. Few if any commercially available compilers support member templates, and few C++ programmers have much experience with STL, so I won’t dwell on this particular option.

The Committee felt that the extra complexity in specifying class `string`, and its fellow travelers, would be more than offset by the richness and conceptual simplicity of the resulting interface. (It will be interesting to see how well that conjecture is supported by actual use of strings in Standard C++.)

## Constructors

To construct a `string` object `x0` with an initially empty character sequence, you can simply write:
```cpp
string x0;
```

Each object `x0` reports the `length` of its character sequence with the call `x0.length()`. (You can also write `x0.size()`, or test for an empty string with `x0.empty()`, for better compatibility with STL.) The object also reports its *reserve size* with the call `x0.capacity()`. Once you’ve constructed a `string` object, you can dictate the minimum number of characters `N` to reserve storage for by calling `x0.reserve(N)`. The function `x0.max_size()` tells you the largest string you can possibly allocate (assuming adequate storage).

For an object with a non-empty character sequence, the reserve size suggests how much storage is currently available to hold the character sequence. Thus, `x0.length() <= x0.reserve()` at all times. As a rule, you can ignore the reserve size. The implementation will guess it for you.

To construct a `string` object and define its initial character sequence, you have a number of choices. In each case below, the string literal in the comment shows the resulting character sequence. The implied null character at the end of this string literal is *not* part of the character sequence. You can write:
```cpp
string x3(5, 'a');       // "aaaaa"
string x4("abcde");      // "abcde"
string x5("abcdefg", 5); // "abcde"
```

You can also define the initial character sequence when you construct a `string` object by selecting a *substring* from another `string` object. Given the definition of `x5` above, you can write:
```cpp
string x6(x5);           // "abcde"
string x7(x5, 1);        // "bcde"
string x8(x5, 1, 3);     // "bcd"
```

A substring thus has two default arguments. In order, these are:
* the initial position `pos` in the character sequence, counting from zero (default is zero)
* the maximum number of characters `n` to include from the remainder of the character sequence (default is `npos`, a huge value)

Finally, you can define the initial character sequence with two iterators, as in STL. An iterator can, of course, be simply a pointer to `const char`:
```cpp
const char a[] = "abcde";
string x9(&a[1], &a[4]);    // "bcd"
```

Note that the second iterator points *just beyond* the last character in the sequence.

## Notation

Class `string` supports numerous combinations of argument types for practically every function that constructs or manipulates such objects. For brevity, I define here a terse and uniform style for writing various argument combinations. The resultant argument is:
* a repetition of characters, with the arguments `rep`, `c`, or a repetition of null characters if you omit the second argument
* a null-terminated array of characters, with the argument `s`
* an array of characters with a specified length, with the arguments `s`, `n`
* a substring, with the arguments `x`, or `x`, `pos`, or `x`, `pos`, `n`, as described above
* a sequence delimited by starting and ending iterators, with the arguments `first` and `last`

With this notation, I can more quickly summarize all the ways you can perform various operations involving `string` objects. Here, for example, is a summary of most of the constructors I showed above:
```cpp
string x3(rep, c);
string x4(s);
string x5(s, n);
string x6(x);
string x7(x, pos);
string x8(x, pos, n);
string x9(first, last);
```

## Altering Strings

A number of member functions alter the character sequence controlled by a `string` object `x0`.

To assign a new character sequence, in place of any existing one:
```cpp
x0.assign(rep, c)      // x0 = c
x0.assign(s, n)
x0.assign(x)           // x0 = x
x0.assign(x, pos)
x0.assign(x, pos, n)
x0.assign(first, last)
x0.assign(s)           // x0 = s
```

To append to an existing character sequence:
```cpp
x0.append(rep, c)        // x0 += c
x0.append(s)             // x0 += s
x0.append(s, n)
x0.append(x)             // x0 += x
x0.append(x, pos)
x0.append(x, pos, n)
x0.append(first, last)
```

To insert before the position designated by index `px` in an existing character sequence:
```cpp
x0.insert(px, c)
x0.insert(px, rep, c)
x0.insert(px, s)
x0.insert(px, s, n)
x0.insert(px, x)
x0.insert(px, x, pos)
x0.insert(px, x, pos, n)
```

To insert before position designated by iterator `p0` in an existing character sequence:
```cpp
x0.insert(p0, c)
x0.insert(p0, rep, c)
x0.insert(p0, first, last)
```

To replace at most `m` characters beginning with position `px` in an existing character sequence:
```cpp
x0.replace(px, m, rep, c)
x0.replace(px, m, s)
x0.replace(px, m, s, n)
x0.replace(px, m, x)
x0.replace(px, m, x, pos)
x0.replace(px, m, x, pos, n)
```

To replace the sequence delimited by iterators `first0` through `last0`:
```cpp
x0.replace(first0, last0, rep, c)
x0.replace(first0, last0, s)
x0.replace(first0, last0, s, n)
x0.replace(first0, last0, first, last)
```

You can remove a substring from a character sequence in various ways:
```cpp
x0.remove()              // remove all characters
x0.remove(px)            // remove all from px to end
x0.remove(px, n)         // remove at most n beginning at px
x0.remove(first0)        // remove element at first0
x0.remove(first0, last0) // remove range
```

You can establish a new length `len` for a character sequence. If the new length is greater than the existing length, the character sequence is padded as stated in the comment below:
```cpp
x0.resize(len)    // pad to len with null characters
x0.resize(len, c) // pad to len by repeating c
```

As a final alteration, you can store a new value `c` in the element at position `px` two different ways:
```cpp
x0.at(px) = c
x0[px] = c
```

And you can access a character with the const companions to these member functions:
```cpp
x0.at(px)
x0[px]
```
both of which return the character value stored at position `px`.

The subscript notation is more efficient, as a rule, because it doesn’t perform a range check on the subscript `px`. The `at` member functions do check, however, if you value safety over speed. In any case, the reference returned can be invalidated by all sorts of subsequent operations on `x0`. Use these forms only in a very localized context, as above.

## Pointers and Iterators

You can get a pointer to the beginning of the entire character sequence, complete with terminating null character, by calling either of:
```cpp
x0.c_str()
x0.data()
```

And you can get various important iterator values:
```cpp
x0.begin()        // first element
x0.end()          // just beyond last
x0.rbegin()       // reverse_iterator first
x0.rend()         // reverse_iterator last
```

(No, I won’t bother to explain about reverse iterators here.) In all cases, the same caveats apply as for the subscript operator, above. Use the pointer or iterator to do any direct accessing of the character sequence quickly, before you perform any subsequent operations on `x0`. The value may well become invalid.

## Copying and Swapping

You can copy into a character array beginning at `s` at most `n` characters from the character sequence, by writing:
```cpp
x0.copy(s, n)     // copy at most n, from the beginning
x0.copy(s, n, px) // copy at most n, beginning at px
```

Yes, the arguments are backwards from the usual way for designating a substring. And you can swap two strings efficiently by writing:
```cpp
x0.swap(x)
```

Some functions each construct a `string` object to return as the value of the function, leaving the object `x0` unaltered. You can, for example, obtain a substring as a separate object:
```cpp
x0.substr()       // copy all
x0.substr(pos)    // copy remainder beginning at pos
x0.substr(pos, n) // copy at most n beginning at pos
```

You can also construct a `string` object that appends the character sequences defined by two operands:
```cpp
x + c
c + x
x + s
s + x
x + x0
```

## Comparisons and Searches

Comparing two `string` objects involves a character-by-character comparison of corresponding values in the two character sequences, for the length of the shorter sequence. If two corresponding values are unequal, the character with the larger value, treated as type `unsigned char`, compares larger. Otherwise, if the two character sequences are of equal length, they compare equal. Otherwise, the longer character sequence compares larger. To compare two complete character sequences for equality (`==`) or inequality (`!=`), you can write any of:
```cpp
x == c            x != c
c == x            c != x
x == s            x != s
s == x            s != x
x == x0           x != x0
```

You can also call the member function `compare` to compare part or all of the character sequences. The comment that follows shows the equivalent comparison in operator notation:
```cpp
x0.compare(x)        // x0 == x
x0.compare(x, px)    // substr(x0, px) == x
x0.compare(x, px, n) // substr(x0, px) == substr(x, 0, n)
```

Yes, the last example does *not* follow the usual pattern for specifying substrings.

A number of member functions look for substrings within a character sequence. To find the first (lowest position) occurrence of the argument character sequence within a character sequence:
```cpp
x0.find(c)
x0.find(s)
x0.find(s, 0, n)
x0.find(x)
```

All such calls return the beginning position of a successful match, or `npos` to report failure. Yes, the third example does *not* follow the usual pattern for specifying substrings. That’s because the second argument, when present, is always the position `px` within the character sequence where the search begins. So to find the first occurrence of the argument character sequence *beginning on or after* `px`:
```cpp
x0.find(c, px)
x0.find(s, px)
x0.find(s, px, n)
x0.find(x)
```

Similarly, to find the last (highest position) occurrence of the argument character sequence within a character sequence:
```cpp
x0.rfind(c)
x0.rfind(s)
x0.rfind(s, npos, n)
x0. rfind(x)
```

Note the use of `npos` to specify a very large position. To find the last occurrence of the argument character sequence *beginning on or before* `px`:
```cpp
x0.rfind(c, px)
x0.rfind(s, px)
x0.rfind(s, px, n)
x0. rfind(x)
```

Four sets of member functions treat the argument character sequence as a *set* of characters. Each character in the set is compared against part or all of the character sequence controlled by the `string` object. For example, to find the *first* element in the character sequence that matches *any* character in the set (`find_first_of`), or that matches *no* character in the set (`find_first_not_of`):
```cpp
x0.find_first_of(c)        x0.find_first_not_of(c)
x0.find_first_of(s)        x0.find_first_not_of(s)
x0.find_first_of(s, 0, n)  x0.find_first_not_of(s, 0, n)
x0.find_first_of(x)        x0.find_first_not_of(x)
x0.find_first_of(c, px)    x0.find_first_not_of(c, px)
x0.find_first_of(s, px)    x0.find_first_not_of(s, px)
x0.find_first_of(s, px, n) x0.find_first_not_of(s, px, n)
x0.find_first_of(x)        x0.find_first_not_of(x)
```

And to find the *last* element in the character sequence that matches *any* character in the set (`find_last_of`), or that matches *no* character in the set (`find_last_not_of`):
```cpp
x0.find_last_of(c)         x0.find_last_not_of(c)
x0.find_last_of(s)         x0.find_last_not_of(s)
x0.find_last_of(s, 0, n)   x0.find_last_not_of(s, 0, n)
x0.find_last_of(x)         x0.find_last_not_of(x)
x0.find_last_of(c, px)     x0.find_last_not_of(c, px)
x0.find_last_of(s, px)     x0.find_last_not_of(s, px)
x0.find_last_of(s, px, n)  x0.find_last_not_of(s, px, n)
x0.find_last_of(x)         x0.find_last_not_of(x)
```

## Input and Output

Finally, you can insert and extract objects of class `string`. For example:
```cpp
cin >> x0
```
extracts a character sequence from the standard input stream and assigns it to `x0`. You can set the width field nonzero in `cin` to limit the number of characters extracted. Otherwise, extraction continues up to but not including the first white-space character.

To extract up to but not including a delimiter character, you can write:
```cpp
getline(cin, x0)     // delimiter is '\n'
getline(cin, x0, c)  // delimiter is c
```

And last of all, you can insert the character sequence into, say, the standard output stream, by writing:
```cpp
cout << x0
```

## Conclusion

As you can see, the draft Standard C++ library offers rather ambitious support for string classes. I confess that there are now far more member functions than I can keep track of, or ever imagine using. Some functions are so overloaded that you dare not write a bald zero as an argument any more. The odds are too great that you will create an ambiguity between a zero integer and a null pointer.

What’s harder to see is that the underlying data structure need not be all that complex. You pay a price at translation time to read all those member definitions, as I indicated earlier. But the execution-time cost of manipulating `string` objects can be reasonably modest. I only wish there were a lightweight flavor of strings to meet more pedestrian requirements, given that template class `basic_string` is now so embellished.

I’ll describe one way to implement string templates [next month](../../13.08/plauger/plauger.md).

*This article is excerpted in part from P.J. Plauger, The Draft Standard C++ Library, (Englewood Cliffs, N.J.: Prentice-Hall, 1995).*
