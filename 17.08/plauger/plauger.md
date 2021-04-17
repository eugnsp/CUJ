# Standard C/C++: A Better List

**P. J. Plauger**

*P. J. Plauger is Senior Editor of **C/C++ Users Journal** and President of Dinkumware, Ltd. He is the author of the Standard C++ Library shipped with Microsoft's Visual C++, v5.0. For eight years, he served as convener of the ISO C standards committee, WG14. He remains active on the C++ committee, J16. His latest books are **The Draft Standard C++ Library**, **Programming on Purpose** (three volumes), and **Standard C** (with Jim Brodie), all published by Prentice-Hall. You can reach him at pjp@plauger.com.*

---

How hard can it be to maintain a linked list? You'd be surprised. Our Senior Editor certainly was.


## Apologia

I'm going to stay in True Confessions mode for at least another month. I began two episodes back by admitting to a number of gaffes I've made over the years in implementing template class `deque`. (See "[Standard C/C++: A Better Deque](../../17.06/plauger/plauger.md)", *CUJ*, June 1999). It is a workhorse container, in the Standard Template Library, that has very desirable properties. You can grow or shrink the sequence it controls at either end in constant time. By contrast, if you add an element to the front of a vector, you have to copy all the previous elements each time, a process whose time grows linearly with the number of elements in the vector. Nevertheless, a deque shares with a vector the ability to access arbitrary elements in constant time. You can subscript into a deque, or climb over it with a random-access iterator, as easily as you can a vector. But the price you pay for this extra flexibility is considerably more elaborate machinery in a deque than in a vector. It's very hard to get right, as I demonstrated much more vividly than I ever wanted to.

Last month, I set out to brag about improvements I had made to template class `_Tree`. (See "[Standard C/C++: A Better Red-Black Tree](../../17.07/plauger/plauger.md)", *CUJ*, July 1999). This is an "under the hood" container that implements red-black trees – ordered binary trees that tolerate a bounded amount of imbalance. The STL template classes `set`, `multiset`, `map`, and `multimap` all rely on `_Tree` as the underlying data representation. A red-black tree supports insertions, erasures (deletions), and lookups all in time proportional to the logarithm of the number of elements in the tree. That makes it a clear winner for maintaining a large collection that can be ordered by some key value stored in each element. The price you pay, once again, is elaborate machinery for maintaining the tree. I got a bug report from Microsoft just as I started writing last month's column. A week later, I had traced down and eliminated problems that had been lurking in my implementation for years.

I knew the job was dangerous when I took it. Standard libraries exist for a purpose – to keep workaday programmers from reimplementing the same code over and over. The Standard C library, for example, is largely the result of several years' accretion of useful functions from the early days of the C language. By the mid 1970s it had assumed a form that most of us would recognize today, and by the mid 1980s ANSI committee X3J11 had pretty much standardized the library in its current form. We committee members identified several reasons for putting a function in the standard library, but one of the most important ones was complexity. Consider `qsort`, for example. It's widely useful, easy to code wrong, and surprisingly hard to code efficiently. It's a natural candidate for putting in a standard library. But somebody still has to write it, and take the heat if it isn't done well. (And I have, on more than one occasion.)

I gave up on writing compilers about ten years ago. They mostly got too complex for me to compete with the big guys as a lone programmer. But I felt that writing libraries mostly solo was still within my grasp. Indeed, the Standard C library is just large enough to be a nuisance to write in full. It requires knowledge from several only loosely related fields, and it requires care in isolating system dependencies. Having acquired the peculiar set of skills needed for this peculiar job, I went into the library writing business with Standard C. (Actually, I sort of backed into it, but that's another story.)

Then along came the C++ standardization effort. It accreted its library in quite a different fashion. The C++ language evolved considerably, in the guise of being standardized, throughout much of the 1990s. And along with it the Standard C++ library grew from a smallish addition to the Standard C library to something formidable indeed. The addition of the Standard Template Library in mid 1994 doubled the size of the original iostreams library, only to be dwarfed by extensive additions to iostreams itself. For a time, I despaired of ever keeping up with the changes and additions. But somehow I did. My reward was to see Microsoft, IBM, and other major players adopt the library I'd developed over so many years.

Now I'm fully in the library business with my latest company, Dinkumware, Ltd. And I no longer work completely alone. Pete Becker and I write, enhance, and package standard libraries for C, C++, and Java. Much of the time, we have fun doing so. But there are also parts that are just plain hard work. Doing battle with the idiosyncracies of compilers eats a lot of time. And checking out bug reports is always scary. Many are pilot error, or problems with known infelicities, that demand a careful reply. But you never know when an innocent query will unearth a nasty flaw in code you once viewed with overweening pride.

Humility is good for the soul, but it is still a medicine that is hard to swallow.

I make a point of writing about past blunders for several reasons. One is personal – I want to be sure that the lessons in humility take. Nothing like public confession to short circuit all those easy rationalizations. Another is to supply cautionary tales. This could happen to you if you let your guard down, or fail to test enough. And still another is encouragement. We all make mistakes. What distinguishes the professionals from the hackers is a willingness to own up to them, fix them, and move on.

Many years ago, I read a delightful book by Edsger Dijkstra – *A Discipline of Programming* (Prentice-Hall, 1973). In it, he presents solutions to a number of problems in programming, from the deceptively simple to the apparently overwhelming. It was tough sledding for me at the time, as is much of his writing. But what I really enjoyed most was his practice of presenting all the false starts he made along the way to solving the various problems. The subliminal message is liberating. If someone of Dijkstra's caliber can make mistakes and own up to them, I guess I can do so too.

And that's why I decided to continue with True Confessions a bit longer.

## List, List, Oh List

The sermon this month centers around template class `list`. It's yet another of the STL template containers. This one manages a controlled sequence as a bidirectional linked list of nodes. Each node stores the element value of interest, plus forward and backward links to adjacent nodes. Having written numerous list managers over the past few decades, I can attest that lists are a prime candidate for inclusion in a standard library. They are widely useful, easy to get wrong, and hard to code efficiently. And unlike deques and red-black trees, they are deceptively simple – which makes them all the more dangerous in their own way.

The straightforward operations on lists aren't too hard to get right. All forms of insertions, deletions (erasures), replacements, and assignments can be expressed in terms of inserting and removing single list nodes. Get those basic operations right and all else follows. But a list has other tempting properties. You can move individual elements or whole subsequences around just by rewriting a few links. No need to copy elements that might be very large. In fact, quite a few of the common STL algorithms can really benefit from this ability to reorder a list with just a modicum of restitchery.

Perhaps the most ambitious example is sorting. Template function `sort`, declared in the header `<algorithm>`, implements a very sophisticated algorithm that's generally hard to beat. But it requires random-access iterators, and a list supports only bidirectional iterators. Wouldn't it be nice if you could somehow take advantage of a list's peculiar properties to implement a reasonable sort just for it?

It turns out that you can. [Figure 1](sort.cpp) shows the simpler form of the `list` member function `sort`. This one orders the elements of the list using `operator<` to compare pairs of elements. There is also a member template version that lets you specify the ordering rule as a function object. It has the same structure, but it's a tad harder to read. Ignore for the moment the lines commented out in [Figure 1](sort.cpp). I'll get back to them later.

I got the basic algorithm from the original Hewlett-Packard implementation. It is a marvel of economy, for which I take no credit. My contribution came in rewriting it to meet slightly different design goals – and not incidentally to understand it better in the process. Sadly, that's also where the bugs came in.

But first, let's look at how the thing is supposed to work. The basic idea is to peel off elements from the list one at a time and merge them efficiently into a sorted list. When you're done, you put the sorted list back in the now empty container. The operations "peel off," "merge," and "put back" all involve unstitching a sequence of one or more nodes from one list and splicing it into another. Template class `list` kindly provides several variations on the member function `splice` to do this very job. So the business of sorting boils down to marshalling a series of splice operations to reorder the list.

Of course it's not that simple. The algorithm I just described is naive. As you merge successive elements into the ordered list, it gets longer and longer. Hence, the average time to place each additional element also gets longer and longer. Perform `N` operations each with linear time complexity (proportional to `N`) and the result takes time proportional to <code>N<sup>2</sup></code>. But we know that the best sort algorithms look more like `N * log(N)`, so there's probably room for improvement.

What the code in [Figure 1](sort.cpp) actually does is a bit trickier. It begins with an array of empty lists. Element zero of the array is a parking place for an ordered list that is never left longer than one element. (So it's pretty easy to keep ordered.) Element 1 grows up to length 2, element 2 grows up to length 4, and so on – the lists doubling in length as you move up the array.

For each element peeled off the original list, the function walks up the array. If it finds a non-empty list, it merges in the new element (with the shortest available ordered list, please note). It then picks up the newly merged list and moves up the array. If the function instead finds an empty list, it plonks down whatever it has accumulated thus far in that parking spot and goes on to the next element in the original list.

When the original list is exhausted, the function merges all the temporary lists to make the final ordered list back in the original container. Job done. I don't know the overall time complexity – still looking for an analysis so I don't get a headache doing it myself – but it's a safe bet that it's rather better than the first naive approach.

## Double or Nothing

I glossed over a small administrative problem with this description. How big should the array of lists be? Clearly, it has to be about <code>log<sub>2</sub>(N)</code> elements (rounded up), since the highest used element has to hold at least half the original list. But C++ won't let you allocate an auto array with a length computed at run time. You have to compute a logarithm of an integer, allocate storage on the heap, and free it to get an array of just the right size.

Another approach is to allocate an auto array that's plenty big enough for all conceivable needs. Indeed, the original H-P implementation simply declares an array of 64 lists. If you recall the story of the grains of wheat on a chessboard, you know the power of successive doublings as you progress from square to square. Those 64 lists should handle lists of around 10<sup>19</sup> elements. Even with today's large computer memories, you're not likely to contrive a list anywhere near that big.

But I balked at the hidden cost of this kind of overkill. I have customers who want to code in C++ even on embedded systems. They take a dim view of functions that take half a kilobyte or more of stack space, particularly if that space is almost always largely unused. So I decided to alter the algorithm to work with a smaller array, but to fail safe if the array proved to be smaller than desirable.

The idea was to set aside the last element of the list array to handle overflow. A list at this location just keeps getting larger; it is never merged up into another list comfortably twice as large. Performance might suffer for an unexpectedly large list, but stack utilization is more reasonable in the general case.

Having made that design decision, and the changes that go with it, I arbitrarily settled on a list array length of 15. Lists of up to 32,768 elements would benefit from the fast algorithm, but longer lists would still end up properly sorted. Great plan, but it had two flaws. First, I made an off-by-one error in writing the code – a not uncommon occurrence in our business. But second, I shipped the code before I stress tested it enough to reveal the problem.

As a consequence, I found out about the bug the worst possible way. Microsoft customers began to notice that sorting a list caused it to lose elements. Specifically, every time the list sort created a temporary list of 32,768 elements, it misplaced it. Not good.

The fix is easy enough. We posted it to our web site last year, in fact. (See http://www.dinkumware.com/vc_fixes for this and other fixes to the VC++ library.) As [Figure 1](sort.cpp) shows, you just have to change the test from:

```cpp
if (_I == _MAXN)
    _Arr[_I].merge(_X);
```

to:

```cpp
if (_I == _MAXN)
    _Arr[_I - 1].merge(_X);
```

You can also tinker with `_MAXN`, which determines the list array size. Just changing it from 15 to 20 lets you sort up to a million list elements efficiently. Set it high enough and you don't even have to fix the bug – just rely as H-P does on having a limit in the stratosphere. But I guess I'm too stubborn to take that easy way out.

With this bug fix, I thought I was in good shape with sorting lists. Then I made the same kind of mistake that got me last month. I set out to describe why `list::sort` now works properly. Only then did I notice that I had introduced a performance bug along with the off-by-one error. For reasons that escape me, I decided to reverse the order in which the list fragments get merged to produce the final sorted list. (Probably I was enamored of the simpler loop logic, and failed to notice the effect on the algorithm.) The original version I wrote merged the list fragments from largest to smallest, thereby causing just the sort of wheel spinning this approach is designed to avoid. As the last comments in [Figure 1](sort.cpp) show, the final merge is not all that more elaborate when done properly.

## A Splice in Time

I thought I was *really* done with template class `list`, until a week or so ago. Then I got a bug report from a moderately sophisticated customer. He found a situation, however rare, where splicing operations mucked up the recorded length of a list. Here's how.

Remember allocators? They're those creatures that manage storage allocation for every STL container (not to mention `string`s, which are not strictly part of STL). The C++ Standard is aggressively ambivalent about these creatures. It says that an implementation does not have to support allocators that are at all ambitious. You cannot, for example, write an allocator template class that portably maintains separate storage pools for different sets of containers. At the same time, an implementation is encouraged to provide just these sorts of extensions. Go figure.

In any event, I endeavored to provide for more elaborate allocators when I wrote all this code. In particular, all the containers test allocator objects for equality whenever that might make a difference. If allocators compare unequal, then it is not safe to allocate storage with one and free it with another. This has implications, for example, with template function `swap`. You can often swap the contents of two containers just by rewriting the pointers they store to the storage each has earlier allocated. But if the containers have incompatible allocators, you have to fall back on the more conventional (and expensive) three-way assignment to swap two containers:

```cpp
Cont temp = x;
x = y;
y = temp;
```

The same consideration applies, in spades, with lists. Here, you can splice individual list nodes among different lists. You can end up with a list that has nodes allocated by any number of other list objects. But you'd better not end up there if these lists don't have compatible allocators.

Splicing operations occur all over the place within template class `list`. So naturally enough, I defined a protected member function `_Splice` to centralize this nontrivial operation. For example, the visible member function `splice`, which splices the entire contents of some list `_X` into this list, looks something like:

```cpp
void
splice(iterator _P, list<_Ty>& _X)
    {if (this != &_X && !_X.empty())
        {_Splice(_P, _X, _X.begin(),
             _X.end(), _X._Size); }}
```

The extra arguments, which define the range of elements to actually move, are there to deal with other ways that `_Splice` can be called. [Figure 2](splice.cpp) shows the current version of `_Splice`. The critical test for allocator equality is given by the cryptic statement:

```cpp
if (_Mybase::_Alval == _X._Alval)
    .....
```

So I tried to get it right. Only trouble was, I didn't think the matter through. The original version of `_Splice` lacked a fifth argument, which specifies how many elements actually move during a splice operation. It seemed easy enough to increment and decrement counts as needed in the calling functions. But I failed to consider what happens when the allocators differ. In that case, the member functions `erase` and `insert` do the work instead, and they both update the node count as needed. Double bookkeeping.

The fix again was easy. Just add the extra argument and let `_Splice` do the whole job, however it sees fit. (This fix is *not* available at our web site, at least of this writing. But please note that it arises only if you use a particularly ambitious form of home-grown allocator, which very few people do to date.)

## Conclusion

I suspect by now you get the drift. Software contains bugs because all us programmers are merely human. Those of us who produce widely used software try to be particularly careful. We test everything we write, and retest it every time we change it. But we don't always think of all the things to test that we should. When that happens, you get to help us debug. And I don't like that situation any more than you do.
