# Questions & Answers: The Complex Evolution of `<string>`

**Pete Becker**

*Pete Becker is Technical Project Leader for Dinkumware, Ltd. He spent eight years in the C++ group at Borland International, both as a developer and manager. He is a member of the ANSI/ISO C++ standardization commmittee. He can be reached by email at petebecker@acm.org.*

---

Pete describes how class string got that way, and how to avoid a few pitfalls in
writing C expressions.

*To ask Pete a question about C or C++, send e-mail to petebecker@acm.org, use subject line: Questions and Answers, or write to Pete Becker, C/C++ Users Journal, 1601 W. 23rd St., Ste. 200, Lawrence, KS 66046.*

> I have some code from a current (1997) C++ college text that looks like:
> ```cpp
> #include <cstring.h>
> #include <iostream.h>
> int main()
> {
>   string MyString;
>   cout << "Enter a string: "
>        << flush;
>   while( cin >> MyString )
>     cout << MyString << endl;
>   return 0;
> }
> ```
>
> I have the VC++ 5.0 compiler and it complains about there not being a `cstring.h`, so I changed it to `<string>` and it claims that string is an undeclared identifier. The compiler doesn’t seem to see that string is supposed to be a type.
>
> Can you shed some more light on this? Is there now a string data type in C++, or is this some sort of template thing? I have been trying to follow the header-file follies related to the C++ Standard as best I can. I have yet to hear anyone actually say that there will now a be a built-in type string.
>
> *Chris Meisenzahl*

Hmm, your textbook writer hasn’t been paying attention to the progress of the C++ Standard. Borland and Rogue Wave used a header named `cstring.h` a few years ago, but once the C++ standardization effort settled in on the name `<string>` for the header, pretty much everyone switched. Also, the use of the manipulator `flush` is not needed here; `cin` and `cout` are synchronized, so your prompt will always appear before the program starts waiting for input. `string` is a genuine type, but it lives in the namespace `std`. You can get at it in either of two ways. The first is as follows:

```cpp
#include <string>
#include <iostream>

int main()
{
  std::string MyString;
  std::cout << "Enter a string: ";
  while( std::cin >> MyString )
    std::cout << MyString
              << std::endl;
  return 0;
}
```

In most cases this is the better way to go. Those `std::` qualifiers may seem a bit tedious, but when you’re using several different libraries with several different namespaces it helps you keep track of where things are coming from. However, compilers and their runtime libraries today don’t uniformly support namespaces, so you may find that some compilers don’t like the `std::` qualifier. They may want to see some of these names standing alone. The compilers will tell you which names they aren’t happy with. Over the short term you’ll see a lot of code that avoids that problem by doing this:

```cpp
#include <string>
#include <iostream>
using namespace std;

int main()
{
  string MyString;
  cout << "Enter a string: ";
  while( cin >> MyString )
    cout << MyString << endl;
  return 0;
}
```

This trick will work, but it defeats the purpose of namespaces. It’s not a good habit to get into, but it may be easier if you have to move back and forth between compilers with varying support for the namespace `std`.

A third approach is to use a macro:

```cpp
#include <string>
#include <iostream>

#if HAS_NAMESPACES
#define STD std
#else
#define STD
#endif

int main()
{
  STD::string MyString;
  STD::cout << "Enter a string: ";
  while( STD::cin >> MyString )
    STD::cout << MyString
              << STD::endl;
  return 0;
}
```

The idea here is to write code that looks like it’s using namespaces appropriately, even when they aren’t actually supported. That way you get in the habit of thinking about namespaces, rather than simply ignoring them as the previous approach does. Yes, there’s definitely a standard type string. Its full name is `std::string`, and sometimes you have to be a bit formal and use its full name instead of using its nickname.

---

> I’ve just started using the draft Standard `string` class. Is there a standard method for reading data into a string? Currently, I’m using
>
> ```cpp
> str.resize(len);              // Allocate memory
> file.read(str.begin(), len);  // Store bytes into buffer
> ```
>
> This works (with Microsoft Visual C++ 5.0), but I don’t think the Standard says it’s supposed to. I could read the data into a char array and then use `str.assign`, but that throws me right back into the oversized array versus managing dynamic memory problems that string was supposed to avoid. Am I the only person who wants to be able to write into a string as if it were an array of char?
>
> *Christopher Madsen*

As you’ve surmised, there are a couple of problems in your code. There’s a much simpler way to do this, but you’re going to have to read through a short lecture on the uses and abuses of containers before you get to it. Unless, of course, you decide to skip to the end, but please don’t do that. I think you’ll find this discussion useful.

The `string` class [1] in the Standard Library was originally designed to provide the operations that the C library provides for null-terminated arrays of `char`, but with the added benefit of automatic memory management. There was also a `wstring` class, which was intended to do the same thing for null-terminated arrays of `wchar_t`. Then someone added one and one and came up with template, and suddenly there was a great deal more code involved in implementing strings. Then someone else said that we ought to make strings into STL containers – after all, the principle of orthogonality demands that we be able to `random_shuffle` a string, even if the principle of practicality says
that this functionality is pointless [2].

So today you can think of the string class as a collection of functions that manipulate text, or you can think of it as an STL container. But don’t try to do both at once, or you’ll end up with confused code. And don’t think of it as an array of `char`. It’s not. It’s just implemented that way.

For any STL container, the member function begin gives you back an iterator that points to the first element of that container. Iterators are designed to work with STL algorithms, and shouldn’t be used for anything else. Any sensible implementation of the standard string class will use a pointer to `char` as its iterator, but there’s no requirement that begin actually return a pointer. All that’s required is that it return some type of object that can be used in the somewhat stylized way that STL uses iterators.

So your code works only by luck: your implementation happens to use a pointer to its internal representation, and that internal representation happens to be an array of `char` that’s suitable for the call to `file.read`. That’s not guaranteed to be the case, though, so it’s not a good habit to get into.

The Standard Library is actually pretty well designed, aside from its excesses. If you find that you’re writing tricky code to do something that ought to be simple, you’re probably overlooking something. In this case you’ve overlooked a somewhat confusing function named `getline`. The reason that it’s confusing is that it has one of the nastiest prototypes I’ve ever seen:

```cpp
template<class charT, class traits, class Allocator>
  basic_istream<charT, traits>&
getline(basic_istream<charT, traits>& is,
  basic_string<charT, traits, Allocator>& str)
```

Fortunately, you don’t have to wade through all that in order to use it. What that prototype says is that `getline` takes an input stream as its first argument and a string as its second argument, and it returns an input stream. The prototype doesn’t tell you this, but the language definition says that it reads characters from the stream into the string until it encounters a newline [3]. `getline` then returns a reference to the input stream. You don’t have to worry about figuring out in advance how much space it needs: `getline` and `string` work together to make sure that there’s enough space to hold whatever you read in. Using getline, your code would look like this:

```cpp
getline( file, str );
```

The key to finding things like this is to forget about a string being like an array of `char`. That’s C, not C++. Instead, think in terms of its interface, and assume that if you want to do something reasonable with the string, there’s probably a function that will do it. Then it’s just a matter of finding that function.

---

> I would like to comment on one of the items in October’s issue of C/C++ Users Journal magazine. There was a question by Mr. M. Zeeshan in the article "[Post-Exception Promises and Uncertainties](../../15.10/becker/becker.md)" on page 98:
>
> I’m using Borland C++ 3.1 & Trident VGA 1 MB, in mode 5D, which is 640x480 with 256 colors. I’m not able to put the pixel after offset address 65,535 (where a segment ends). I’ve tried both far and huge pointers. In the far pointer it assumes 65,536 as 0 and puts the pixel on the (0, 0). I defined the pointer as `(unsigned char far*` & `unsigned char huge*)`. Please tell me what I should do.
>
> VGA is an extension to EGA, so it has some limitations inherited from EGA. The address space for VGA cards is from `0xA000:0x0000` to `0xA000:0xFFFF`. As you can see, this space is only 64 KB = 65,536 bytes. There are many VGA and SVGA modes that require more than 64 KB per video page. So to enable use of more than 64 KB video memory, it is organized in bit-planes. VGA has four bit-planes and each bit-plane consists of 64 KB. Thus, standard VGA has 256 KB video memory; SVGA has more bit-planes (and more video memory).
>
> At any time the programmer can access only one bit-plane (no more than 64 KB). You can switch between bit-planes by directly programming the video controller. Mr. Zeeshan’s problem is that he wants to use 640x480 with 256 colors mode. This mode requires 640x480 = 307,200 bytes, which is more than 64 KB (one bit-plane). So he has to program the video controller to switch between bit-planes to access the entire video memory.
>
> I recommend that Mr. Zeeshan use a graphics library. He uses Borland C++; Borland has a good graphics library. Its limitation is that it supports only the standard VGA modes. For SVGA modes he can use another BGI driver called SVGA256.BGI. I don’t know Mr. Zeeshan’s email address. So, if you can, forward this message to him.
>
> *Stefan Tchekanov*

I received a similar note from Christopher Jennnings. If I were in a bad mood I’d answer both of you by saying "yes, but he didn’t ask how to use the VGA card, he asked how to write beyond offset 65,535." And since it’s my column I could declare victory and that would be the end of it. But unfortunately, you’re right, I didn’t read his mail carefully enough. I answered the question that he asked instead of answering the question that he should have asked. Thanks for catching that, and thanks for the lucid explanation of the basics of VGA memory management. I don’t have Mr. Zeeshan’s address any longer, but if he’s still reading this column despite my bad advice, I hope he sees this.

---

<a name="order-of-evaluation"></a>

> I may be missing the boat here but I’m not sure whether your interpretation
is correct. I’m refering to your [October 1997 column](../../15.10/becker/becker.md), in which you discussed order of evaluation of expressions. Consider the expression:
>
> ```cpp
> y = x + x + x++;
> ```
> According to K&R (I’m not sure if this is ANSI C or not), `x++` should not increment `x` until after the statement has executed; that is, it should execute it like this:
>
> ```cpp
> y = x + x + x;
> x = x + 1;
> ```
> which doesn’t contravene your C Standard clause. I thought that was the whole point of having a post-decrement and pre-decrement operator! If that is truly the case then the Borland compiler evaluated it correctly and all the others were wrong. I’m not sure whether this is Standard C but it’s the way I always interpreted the pre/post increment/decrement operators.
>
> *Michael Dennis*

I wasn’t able to find an analysis of this expression in K&R. What K&R does say, along with every textbook that discusses increment operators, is that the statement

```cpp
y = x++;
```
executes like this:

```cpp
y = x;
x = x + 1;
```

That doesn’t help you understand what happens when you use `x` more than once in the statement, however. The C language definition explicitly prohibits using `x` in the way that the earlier example uses it. But that’s the subject of my earlier column.

Most people learn programming by developing general rules that tell them how various operations in a programming language work, like the one you cited – that `x++` should not increment `x` until after the statement has executed. The problem that we all run into is that the rules we develop might not actually be correct. In this example, your rule is often close enough, but in fact it’s not correct. The actual rule from the C language definition is that the value of the expression `x++` is the original value of `x`, and after that value is used, `x` will be incremented. The key here is nderstanding that the increment can happen any time after the compiler has copied the value out of `x`. It doesn’t say that the increment only happens after everything else in the
statement has been done. Let’s take a different example:

```cpp
#include <stdio.h>

int x = 0;

void show( int i )
{
  printf( "%d, %d\n", x, i );
}

int main()
{
  show( x++ );
  return 0;
}
```

One way a compiler could interpret this program would be to get the value from `x`, increment `x`, and pass the old value to show. If the compiler did that the output would be `1, 0`. Another way to interpret this code would be to get the value from `x`, pass the old value to show, then increment `x`. If the compiler did that the output would be `0, 0`. That’s what your rule would require. Both of these interpretations satisfy the rule that the increment is done only after the value of `x` has been used. Each of them is reasonable.

Only the first interpretation is valid in C, because there is a sequence point prior to the function call. The presence of the sequence point mandates that the increment, and any other side effects, must occur before making the function call.

It would certainly be possible to design a language that did things the way you describe, and it would be a perfectly reasonable and consistent language. The designers of C chose not to do that. There’s not much point in debating which approach is better, because the decision has already been made, and it won’t be changed. It might not fit the way you’re used to thinking about things, and it might not be what you’d have chosen if you were designing a language. Nevertheless, it is the way the language works.

Be very careful about applying rules that you’ve used for years: they might not be right. Or, as they say in the army, if the map doesn’t agree with the terrain, trust the terrain.

---

<a name="conditional-operator"></a>

> Aahhh! Now I’m beginning to understand a similar problem I first came across in Windows programming; trying to alter a bit in an integer (e.g. `a`) according to which radio button was clicked (e.g. `b`):
>
> ```cpp
> #include <stdlib.h>
> #include <stdio.h>
>
> int main()
> {
>     int a = 7;
>     int b = 6;
>
>     // if (b == 6) set bit 1 else clear bit 1
>     (b == 6) ? a |= 2 : a &= ~2;
>     // prints a = 5 (wrong)
>     printf ("Test 1: a = %d\n", a);
>
>     a = 7;
>     // if (b == 5) set bit 1 else clear bit 1
>     (b == 5) ? a |= 2 : a &= ~2;
>     // prints a = 5 (right)
>     printf ("Test 2: a = %d\n", a);
>
>     a = 7;
>     // if (b == 6) set bit 1 else clear bit 1
>     (b == 6) ? (a |= 2) : (a &= ~2);
>     // prints a = 7 (right)
>     printf ("Test 3: a = %d\n", a);
>
>     a = 7;
>     // if (b == 5) set bit 1 else clear bit 1
>     (b == 5) ? (a |= 2) : (a &= ~2);
>     // prints a = 5 (right)
>     printf ("Test 4: a = %d\n", a);
> }
> ```
> The results are from the Borland C3 compiler. MS VC1.5 gave similar results and a version of MS QuickC refused to compile it. If the `if..else` construct is used instead of the conditional operator, there appears to be no ambiguity.
>
> I think this is a more subtle example than the `x + x + x++` you quoted (October 1997 issue). The limited information I have on the conditional operator tends to imply that it can be used as a direct replacement for `if..else`, which is apparently not quite true.
>
> *Tim Bolt*

This is actually a different issue than the one presented by `x + x + x++`. The problem here lies in misunderstanding how the conditional operator works, which is a fairly common condition. Changing to `if ... else` fixes the problem, not because the conditional operator produces an "ambiguity," but because it is being used incorrectly. Please don’t use the word "ambiguity" indiscriminately. There is no ambiguity in your code – it simply doesn’t do what you thought it would do.

The conditional operator is an expression, not a statement. It is evaluated by the compiler, and it produces a result. The `if ... else` construct is a control statement. It does not produce a value; it controls which of its two statements gets executed. Keep this distinction clear in your mind and you’ll avoid many of the mistakes that result from attempts to use the conditional operator. If you want to select which statement to execute, use `if ... else`. Your code is most naturally written like this:

```cpp
if( b == 6 )
    a |= 2;
else
    a &= ~2;
```

You could also use the conditional operator in a straightforward way to do the same
thing, like this:

```cpp
a = (b==6) ? a|2 : a&~2;
```

Here, the conditional expression selects one of the two values, `a|2` or `a&~2`. The
resulting value is assigned to `a`.

When you try to go a step further you can easily get into trouble, as you’ve seen. That’s because the conditional operator has higher precedence than the assignment operator. That is, when there’s an assignment operator to the right of a conditional operator, the conditional operator grabs the expression up to the assignment. That makes it useful for things like this:

```cpp
(b == 6) ? a : c = 3;
```

This statement [4] assigns the value `3` to either `a` or `c`, depending on the value of `b`. And that’s exactly the reason you get unexpected results from your use of the conditional operator. The compiler sees it as if you had written your original statement like this:

```cpp
((b == 6) ? a |= 2 : a) &= ~2;
```

That is, if `b` has the value `6`, it performs the operation `a|=2`, and uses the result of that operation as the left operand for `&=~2`. When `a` starts out as `7`, `a|=2` has no effect, because bit two is already set. Then the compiler evaluates `a&=~2`, which lears bit two, resulting in the value `5` for `a`. If `b` does not have the value `6`, the conditional operator selects the part after the colon, that is, `a`, and uses it as the left operand for `&=~2`. Again, the result is that `a` gets the value `5`. Your tests 3 and 4 produce the result you expected because the parentheses override the default precedence of the conditional operator. But once you’ve written the fully parenthesized expression that’s needed here, you may as well write `if ... else`. It will be clearer.

Finally, note that the above analysis changes if you’re using C++. C++ changed the binding of the conditional operator so that your tests 1 and 2 will do what you expected them to do. That makes the operator more symmetrical, and avoids this sort of surprise. However, if you’re used to the C rule, this change will be surprising. If you’re in a situation where you write both C and C++ code you’ll probably find this difference in semantics rather confusing. That’s yet another reason to use `if ... else` here – it means the same thing in both languages.

## Notes

1. Please don’t write to me to tell me that `string` isn’t a class. I know it’s a `typedef` for a specialization for type `char` of the template `basic_string`, but that’s too long a name, so I’ll just call it the `string` class. The difference is rarely significant.
2. This description of `string`’s evolution might make you think I was opposed to these changes. In fact I was, but now that I’m in the business of developing libraries, the more complex the Standard Library is, the better. Ahhh, job security.
3. There’s another version of `getline` that takes a third parameter whose type is `charT`. If you use that version, the third parameter is the delimiter: the function reads characters until it encounters that character.
4. I know, I said that the conditional operator is not a statement. That’s correct, but any expression can be used as a statement. That’s why an expression like `a=5` can stand on its own as a statement. The same thing applies to the use of the conditional operator here.
