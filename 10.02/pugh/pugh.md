# Questions & Answers

**Ken Pugh**

*Kenneth Pugh, a principal in Pugh-Killeen Associates, teaches C language courses for corporations. He is the author of C Language for Programmers and All On C, and was a member on the ANSI C committee. He also does custom C programming for communications, graphics, image databases, and hypertext. His address is 4201 University Dr., Suite 102, Durham, NC 27707. You may fax questions for Ken to (919) 489-5239. Ken also receives email at kpugh@dukemvs.ac.duke.edu (Internet).*

---

<a name="conditional_operator"></a>

> Our company has banned the use of the conditional expression operator `?:`. What's wrong with it?
>
> *Bill Malone, Worcester, MA*

This same question has floated around Usenet. As I see it, the problem is not with the operator itself, but with the potential for combining it into expressions which become indecipherable.

The operator itself is not necessarily bad. In my C classes, I usually suggest avoiding it as there is the potential for confusion. I give the students an example such as:
```c
i = (7 > 5 ? (10 > 9 ? 4 : 7)
   : (3 > 1 ? 6 : 2);
```

Most of the students take a few minutes to determine the value assigned to `i`. The solution can be found using `if`s in only few seconds.
```c
if (7 > 5)
   if (10 > 9)
      i = 4;
   else
      i =7;
else
   if (3 > 1)
      i = 6;
   else
      i = 2;
```

The feeling of complexity is not universal. A group that I taught at Lotus found the conditional operator rather simple, since its form matches that used by 1-2-3 formulas.

I once did a port of a C program from an IBM-PC to an Apple II. As the program was compiling, it aborted with an error message that it was out of expression space. The problem was with an expression that used the conditional operator. The creator of the program must have fallen in love with this operator. The expression had the operator nested ten deep. Luckily the compiler had an option to increase the expression space and I did not have to rewrite the statement.

One example of the conditional operator that was discussed on the net looked like:
```c
strcpy (foo, x ? xvar : yvar);
```

This was deemed preferable by some to:
```c
if (x)
   strcpy (foo, xvar);
else
   strcpy (foo, yvar);
```

Another example on the net looked something like Listing 1:
```c
printf("The value is '%s' \n", (x == 0 ? "zero" :
                          (x == 1 ? "one" :
                          (x == 2 ? "two" :
                          "lots")));
```
Some responders liked the compactness of this statement. A proposed alternative for code similar to Listing 1 was code like that seen in Listing 2:
```c
char *out_string;
switch (x)
   {
   case 0:
       out_string = "zero";
       break;
   case 1:
       out_string = "one" ;
       break;
   case 2:
       out_string = "two" ;
       break;
   default:
       out_string = "lots";
       break;
    }

printf ("The value is '%s'\n", out_string);
```

Some people felt that Listing 2 was too wordy and had other problems. However, I would propose an even more complicated, but more maintainable organization (to my mind). This is shown in Listing 3:
```c
int index;
#define SIZE_OUT_STRINGS 4

char *out_strings[SIZE_OUT_STRINGS]
    = {"zero", "one", "two", "lots"};

if (x < 0 || x >= SIZE_OUT_STRINGS - 1)
    index = SIZE_OUT_STRINGS - 1;
else
    index = x;
printf ("The value is '%s'\n", out_strings[index]);
```


I feel Listing 3 is more maintainable for several reasons. First, the values of the string might be used in more than one place in the program. Keeping them in a data item makes them accessible to any statement that needs them.

Second, this is closer to the organization which most windowed systems now employ. There is a separation of data from logic. The strings themselves could be read in from a resource file, if that were available. Notice that if you keep all your strings in the declaration portion of the program, a maintainer can easily find them for translation into another language or simply another phrase in the same language.

The arguments on the net ranged around the ideas that "if it's in C, then a C programmer should use it. If you don't understand it, then you shouldn't be programming in C". My response to that is that I can create monsters of expressions that are indecipherable using C operators. Just because a feature is there does not mean I have to abuse it.

---

> The K&R Book says (on page 37) that all automatic variables are initialized each time the function is entered. I have a piece of a program below in which `p` is initialized only once:
> ```c
> char *getstr();
> main()
>    {
>    char *str;
>    printf("%s\n", str = getstr()); /* prints "Old Value" */
>    strcpy(str, "New Value");
>    printf("%s\n", str = getstr()); /* prints "New Value" */
>    }
>
> char *getstr()
>    {
>    char *p = "Old Value";
>    return p;
>    }
> ```
>
> Please explain what I am missing here. The answer that I can think of is that the following two declarations
> ```c
> char *p = "Old Value";
> ```
> and
> ```c
> static char *p = "Old Value";
> ```
> are equivalent. Are they really? Also this behavior is not observed for other types of pointers.
>
> *Abhay B. Joshi, Syracuse, NY*

The two declarations are sort of equivalent. Regardless of which one you use you will get the same value. The variable `p` is a pointer to `char`. The string `"Old Value"` is a string constant, or as I like to describe it, an unnamed array of `char`s identified by an address.

When you initialize a pointer to `char` with a string, the address of that array of `char` is stored into the pointer. The difference between your static and non-static declarations is that this initialization takes place either every time the function is entered (for the automatic variable) or just once (for the static variable). If your function looked like
```c
example_function()
   {
   char *p = "Old Value" ;
   static char *ps = "Old Value";
   printf("p is %s ps is %s", p, ps);
   p = "Another Value";
   ps = "Another Value";
   }
```
then the first time through it would print `Old Value` for both variables. Each subsequent time, it would print `Old Value` for `p` and `Another Value` for `ps`.

Now let's take a look at what your main function is doing. `getstr()` returns the value of `p`, which contains the address of `Old Value`. You perform a `strcpy`. This copies `New Value` to the address where `Old Value` is stored. You have wiped out the constant string.

The next time `getstr()` returns, it returns the same address. But the contents of that address is now `New Value`.

---

> How does one truncate a file under UNIX. I'm looking for a routine similar to DOS's `chsize()` routine that will allow me to set the size of a file. The size that I pass to the function needs to be smaller than the length of the file itself (i.e., shorten it)
>
> Would appreciate any snippets of code, etc. Thanks.
>
> *Brooks Cutter, Largo, FL*

BSD UNIX has `ftruncate(fd, length)`, which works just like `chsize()`. It also has `truncate(path, length)`, which applies to a non-opened file. However, if you are using System V, that doesn't help too much. I suggest something like [Listing 5](listing5.c), which works on a non-opened file. If you do not need to worry about multiple links to the file, then you could simplify it by eliminating the copy back to the original file. You would unlink the original file and then rename the temporary file.

To avoid a lot of nested logic, I have used `goto`s. `goto`less coders are invited to send in their own version. Using multiple `return` statements counts the same in my book as using `goto`s.

---

> In the report provided by `CHKDSK` there is something called "Volume Serial Number". The `format` utility also reports this same thing and evidently `format` creates a new volume serial number each time a disk is formatted. My question is this, where is that serial number stored and what is its intended use?
>
> *Rob Buck, Fairfield, IA*

I can give you an exact quote from the *MS-DOS Programmer's Reference Manual*. I couldn't say it better myself.
> To help distinguish one removable disk or tape from another, the format command creates a unique identifier for each volume ... as it formats the volume. Programs can also create their own unique identifiers by using Set Media ID (Interrupt `21h`> Function `440Dh` Minor Code `46h`) to set the volume label, volume serial number, and file-system type.
>
> Since the user can change the volume in a removable-media drive at any time, programs that read from or write to removable media need ways to prevent inadvertently reading from or writing to the wrong volume. Some drives have change-line capability that helps MS-DOS automatically detect media changes ... If a drive does not have change-line capability, MS-DOS checks for the proper volume before read and write operations.

I'm not sure which versions of MS-DOS really do this volume checking. Version 3.2 does not (at least how I would expect). You can do a simple check by typing
```
more < a:long_file
```

At the pause, remove the disk and put in a different one. You should get either garbage (after a few more screenfuls) or a warning message. Version 3.2 gave me garbage.

## Readers' Responses

### `float`s And `double`s

> I just finished reading your response to my previous note in the September C Users Journal, and I'm afraid we're not communicating. Consider the following code
> ```c
> sub1()
>    {
>    float f;
>    sub2(f);
>    }
>
> sub2(newf)
> float newf;
>    {
>    sub3(&newf);
>    }
> ```
>
> Now, according to both K&R and ANSI, when `sub1` calls `sub2`, `f` is converted from a `float` to a `double` and the `double` is passed to `sub2`. The key question is – exactly what happens in `sub2`?
>
> The declaration of `newf` is `float`, but the parameter being passed is actually a `double`, so what happens? K&R says that the declaration is "adjusted to read `double`". Thus, despite the fact that the programmer declared `newf` as a `float`, the compiler takes it upon itself to quietly "adjust" that declaration and thus make `newf` a `double` instead of a `float`.
>
> In this case, `sub3` gets passed a pointer to `double` which is clearly not what the programmer intended. A number of compiler implementers adopted an alternative solution to the conflict. Rather than adjusting the declaration of `new`, they actually convert the `double` parameter back into a `float`.
>
> That is, they behave as if the function had been written,
> ```c
> sub2(temp)
> double temp;
>    {
>    float newf = temp;
>    sub3(&newf);
>    }
> ```
>
> In this case `sub3` is passed a pointer to `float` as the programmer intended, but there may be extra work involved in converting the `double` back into a `float`. (There also may not be. Some floating point formats allow a `double` to be "converted" to a `float` by just ignoring the extra bytes.)
>
> Since this approach seems to be more faithful to what the programmer asked for, it is the one that was adopted for the ANSI standard. (Although I believe that the "as if" rule would still allow the adjustment as long as the variable never has its address taken.)
>
> *Larry Jones, Milford, OH*

I think we are in agreement on this, except for a particular compiler's implementation of K&R. The question revolved around the Microsoft compiler. For your example, which uses the K&R style function header, Microsoft treats the parameter declaration as if it were stated as `double`. The variable `newf` is eight bytes long.

If you declare it with the new style function header as:
```c
sub2(float newf)
   {
   sub3(&newf);
   }
```
it would be truly a `float`.

In this case, you would need to include a prototype for `sub2` for the calling routine, e.g.:
```c
int sub2(float newf);
```

This would prevent the default widening of `f` in `sub1`. If the prototype is not there, then the function may fail (ANSI Rationale 3.3.2.2).

As stated in the Rationale section 3.7.1, this type rewriting of parameters from `float` to `double` is no longer permissible. However Microsoft appears to have left the old style alone. This is non-ANSI behavior and should not be allowed if the compiler is run in ANSI-compliant mode. (KP)
