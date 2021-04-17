# Standard C/C++: A Singly Linked List

**P. J. Plauger**

*P. J. Plauger is Senior Editor of **C/C++ Users Journal** and President of Dinkumware, Ltd. He is the author of the Standard C++ Library shipped with Microsoft's Visual C++, v5.0. For eight years, he served as convener of the ISO C standards committee, WG14. He remains active on the C++ committee, J16. His latest books are **The Draft Standard C++ Library**, **Programming on Purpose** (three volumes), and **Standard C** (with Jim Brodie), all published by Prentice-Hall. You can reach him at pjp@plauger.com.*
---

Every design involves tradeoffs. The trick is to tradeoff using much the same principles as for related designs.

## Introduction

The most recent C++ standards meeting was held last November in Kailua-Kona, Hawaii. Yes, the committee actually got work done – quite a lot in fact. Nobody spent the week lying on the beach, or training for the Iron Man Triathalon for that matter. (It went right by the conference hotel while we were there.) Tana and I spent the weekend in the middle of the meeting visiting our kids, both of whom happened to be in the islands at the time. (A few minutes with a globe will tell you that Hawaii is as far away from their parents in Massachusetts as they can get and still remain within the 50 United States. Coincidence? You decide.) But we still found time to attend at least one evening session.

Beman Dawes hosted a special Monday evening session. Officially, it was a meeting of Boost (see http://www.boost.org), which serves as a clearing house for additions to the Standard C++ library. It was thus pointedly kept separate from the daily sessions of the C++ standards committe, and for a good reason. The primary order of business was to discuss hash tables, particularly in the form of a significant addition to the Standard Template Library container classes that are already part of the C++ Standard. Since the ink is still drying on the formal C++ Standard, now is *not* the time for the committee to begin considering extensions. But nevertheless, now is a very good time indeed for the C++ community to begin exploring the kind of extensions that might be proposed a few more years down the pike.

Dave Musser, one of the original designers of STL, worked up a template container for hash tables several years ago. He even proposed it as an addition to the draft C++ Standard while it was still being developed. But the committee, in a rare burst of conservatism, twice voted not to take on any more additions to the Standard C++ library so late in the process. Hash tables never made it in.

Nevertheless, hash tables are pretty important to your working programmer. They promise constant access time to an arbitrarily large collection of keyed data. All you need is a reasonably well behaved hash function, over the keys, and a container class to take advantage of it. So quite a few implementations of the Standard C++ library also include Musser's hash tables as a pure extension. I use the plural because these hash tables come in four flavors:

* `hash_set` – where the key is the stored value and duplicate keys are not permitted
* `hash_multiset` – where the key is the stored value and duplicate keys *are* permitted
* `hash_map` – where the key is paired with a separate stored value and duplicate keys are not permitted
* `hash_multiset` – where the key is paired with a separate stored value and duplicate keys *are* permitted

Of course, the Standard C++ library already includes the template classes `set`, `multiset`, `map`, and `multimap`, with much the same properties as their hash-table analogs above. These are all based on an underlying red-black tree, typically, which promises logarithmic time for inserts, deletes (erasures), and lookups. Hash tables can do much better for lookups, with rather less code complexity in the bargain, at the risk of decaying to linear time for lookups with a bad combination of hash function and actual stored data. But most of the time, hash tables win big over the more complex tree-based data structures. It is no accident that Musser structured them as near drop-in replacements for the existing tree-based containers.

Beman's evening session wasted little time on whether or not hash tables should be added to a future revision of the C++ Standard. The attendees were presold on that notion. Rather, the discussion mostly involved the external interface presented by any template containers that implement hash tables. I mentioned above that Musser's implementation is widely used, but it is not the only one currently available. I also wrote one a year or two ago, which I discussed in these pages earlier. (See "[Standard C/C++: Hash Tables](../../16.11/plauger/plauger.md)", *CUJ*, November 1998). My company, Dinkumware, Ltd., has been delivering this addition to compiler vendors as part of our Dinkum C++ Library. We also make it available over the Internet to individuals as part of an upgrade package for Microsoft Visual C++.

I didn't match exactly Musser's external interface. Instead, I tried to make the hash tables I wrote look even more like the existing set and map containers. For example:

* I used `operator<` for an ordering rule, just like the existing set and map containers, but unlike the more traditional `operator==` that Musser used. This makes it possible to define the member function `equal_range` sensibly. For a hash multiset or a hash multimap, the function delimits the range of all elements with equivalent keys.
* I used a bidirectional linked list for the underlying representation, rather than a singly linked list. Thus, the hash-table containers support bidirectional iterators, just like all the existing containers in the Standard C++ library.
* I used an incremental growing strategy, eliminating the need for parameters to specify when to rehash the entire table.

The good news is that the differences matter less than you might think. If you stick with the default template parameters, and just the basic operations on hash tables, the two implementations are largely interchangeable with each other and with the existing tree-based containers. The design decisions are mostly of interest to the more advanced users of hash tables. And those are just the sort of fine points that people must eventually agree on before hash tables are added to a future revision of the C++ Standard. And that is why now is the time to experiment with different, but reasonably compatible, variations, so we can all have the benefit of experience before we make the tough decisions later on.

I was pleased by the discussion at that evening session. The attendees made a real effort to understand both designs, and the reasons behind the differences. I was also pleased that the differences were not a point of contention. There was no sentiment expressed that the first implementation should become an instant standard. Indeed, even the current version of Musser's original template classes has been improved by the folks at Silicon Graphics. Controlled innovation can be a Good Thing. Above all, I was pleased that some of the design choices I made were well received. The differences were not viewed as simply arbitrary.

## I've Got a Little List

I returned from that meeting all the more eager to consider more extensions to the Standard C++ library we license. At the top of the list, as it were, was a template container that maintains a singly linked list. It too is widely available as a nonstandard extension to the Standard C++ library. It even serves as the base for Musser's hash tables, as I indicated above.

On the face of it, the Standard C++ library has no need for such an addition. It already has a bidirectional linked list (template class `list`), which offers a superset of the functionality you can get from a singly linked list. But there are two reasons why you might still want a singly linked list:
* A singly linked list stores just one link along with the element value in each list node. A bidirectional linked list obviously needs two links. Under some circumstances, the reduced storage requirement just might be significant.
* A singly linked list probably has simpler code for inserting and erasing elements. Under some circumstances, the reduced execution time or code space just might be significant.

The obvious price you pay, over a bidirectional linked list, is in the iterators. A bidirectional linked list, obviously enough, supports bidirectional iterators; but a singly linked list supports only forward iterators. Reverse iterators also go out the window, of course, because they need at least bidirectional iterators underneath to do their magic.

I managed to convince myself, years ago, that the potential savings were too marginal to warrant a whole 'nother template class. Instead, I focused on improving the existing bidirectinal linked list, as I also described earlier in these pages. (See "[Standard C/C++: A Better List](../../17.08/plauger/plauger.md)", *CUJ*, August 1999). Nevertheless, some customers still request singly linked lists. As a vendor, you don't have to provide everything your customers request – indeed, you seldom can – but they don't have to be your customers either. So I was willing to rethink my earlier decision, particularly given the popularity of hash tables as a nonstandard extension.

And that was when I walked smack into a more fundamental problem of singly linked lists. For the sake of simplicity, I summarize it as the "before" problem. It comes in three major flavors:
* A universal member function for inserting a new element into an STL container looks like `insert(where, value)`. Here `where` is an iterator that designates the element *before* which the new element is to be inserted. But for a singly linked list, `where` denotes the element just beyond the one whose link must be updated.
* A set of member functions peculiar to template class `list` allows you to splice in a sublist from a list object. It can even be a sublist from elsewhere in the same list object. But once again, a call such as `splice(where, other_list)` takes an iterator `where` that designates the element *before* which the sublist is to be respliced.
* A universal member function for erasing an element in an STL container looks like `erase(where)`. Here `where` designates the element to be erased. But once again, to perform the erasure you need to update the link in the element `before` the one designated by the iterator.

There's only one way to back up an iterator within a singly linked list. Begin at the beginning of the sequence and chain down, stopping just short of the element whose predecessor you seek. This is obviously an operation that takes linear time – its execution time increases in direct proportion to the number of elements in the controlled sequence. STL is carefully designed to depend only on constant-time iterator operations, so this is a serious overhead. You can't insert, splice, or erase in constant time if the operation requires an iterator calculation that takes linear time.

One solution to this problem is simply to accept the lousy time complexity. You can add nonstandard member functions with names like `insert_after`, `splice_after`, and `erase_after` that do the obvious. They each take an iterator that designates the element `before` the usual place in the sequence – the place you really care about when manipulating a singly linked list.

I find this approach graceless, however. The universal STL notation was chosen for a darned good reason. It makes sense to talk about the position `before` any iterator value between `begin()`, which designates the first element in the sequence, and `end()`, which designates the place just beyond the last element in the sequence. For an empty sequence, `begin()` equals `end()` and all such iterators still make sense. But this is not true for the "`after`" functions above. There is no way to specify the very beginning of the list. Worse, it makes no sense to insert something just after the place already beyond the end of the list. You can patch both problems, and people have, but the result is still a patchwork. And even in the best of times it is all quite different from the existing family of STL containers.

So I chose a different approach. A typical list (or red-black tree) iterator stores a pointer to the designated node. But I replaced this pointer with something rather different, a pointer to a pointer to the designated node. Let's say that the list container stores a pointer `_First` that designates the first node in the list. If the list is empty, then `_First` stores a null pointer. Say further that each node contains a pointer `_Next` that designates the next node in the list. If the node is at the end of the list, then `_Next` stores a null pointer. Finally, each iterator stores a pointer to pointer `_Ppnode`. With this notation:
* `begin()` returns an iterator that stores `&_First`.
* `++iter` increments the iterator `iter` by replacing the value stored in `_Ppnode` with `&_Pptr->_Next`.

[Figure 1](sliter.h) shows the definition of the nested class `const_iterator`, which implements a constant iterator over a singly linked list template container `slist`. It also shows a passel of the type definitions required of all STL containers, to put matters slightly more in context. It does fulfill the promise of being dirt simple. Nevertheless, the iterator can at all times deliver the address of the link that needs updating for all the common mutations of a list. As a result, this implementation of a singly linked list fulfills the conventional promise of constant time inserts, erasures, and splices.

## A Walk on the Down Side

There is, as always, a catch. The down side of this approach is that an iterator peers inside the list node *before* the one it actually designates. That's fine so long as everything holds still. But consider:
* If you insert an element before the one designated by the iterator `where`, then `where` now designates the newly inserted element instead. (Think about it.)
* If you erase the element before the one designated by `where`, then `where` is now an invalid iterator.

Surprising behavior, you will say, and hardly desirable. One of the virtues of an iterator into a map, set, or bidirectional linked list is that it remains valid as long as the element it designates continues to exist. It can even be spliced about, in the case of a list, and still be valid. Only when you increment or decrement such an iterator does it bother to determine who its current neighbor is and chain over to that neighbor. Nice properties.

A "`before`" iterator, however, requires more careful usage idioms. You even have to beware such common idioms as:

```cpp
for (iter = first; iter != last; ++iter)
   if (<condition>)
      iter = cont.erase(iter);
```

This code is careful to capture the successor to `iter` after an erase, rather than guess how to find the successor by hand, as it were. For all existing STL containers, it's safe as houses. But for a singly linked list, you face another danger. If the body of the loop erases the last element of the sequence, then `last` becomes invalid before the loop terminates. If the freed node gets scribbled on, as is often the case, all hell can break loose.

Please note, however, that this problem doesn't occur if you're scanning the entire container, as in:

```cpp
for (iter = begin(); iter != end(); ++iter)
   if (<condition>)
      iter = cont.erase(iter);
```

Since `end()` is recomputed each time, it can return a valid value. But you must resist the temptation to optimize away all those calls to `end()` by calling it once and storing the result. As it turns out, `end()` is really a cheap function to call most of the time. It gets expensive only when you really need it to redetermine the end iterator anyway.

[Figure 2](slinsert.h) shows the family of insert functions I wrote for template class `slist`, and [Figure 3](slerase.h) shows the family of erase functions. In each case, you can see that the code has to get finicky, from time to time, in how it advances iterators and tests for completion. Luckily for the user of this class, most such trickery can be confined to member functions within the template class. You need to indulge in such stupid iterator tricks only when you take it upon yourself to write a loop that erases or inserts elements as it goes along. Experienced users of STL template containers already know that such operations require the utmost care, anyway.

Space does not permit me to show the entire template class here. All I will say is that this version of `slist` has *all* the member functions of template class `list`. You can even perform sorts, merges, and list reversals with the same time complexity as for `list`, believe it or not. I was pleasantly surprised to discover that myself. But given the constant time insert, erase, and splice, everything else falls into place. All you have to sacrifice over `list` is bidirectional iterators and reverse iterators.

So the final question is whether the tradeoff is appropriate. Which is the more important principle to uphold in an STL container, proper time complexity or simple iterator behavior? I came down in favor of the former for two reasons. First, time complexity requirements are pretty rigorously honored throughout the STL classes and algorithms that made it into the Standard C++ library. Second, iterator behavior is already questionable in several other places. For example:
* Inserting an element at the beginning of a deque invalidates an earlier iterator returned from `begin()`, and inserting an element at the end invalidates an earlier iterator returned from `end()`. Template class `vector` has similar problems, but *not* a map, set, or bidirectional linked list. And `vector` has further problems with internal insertions. You already have to be careful when switching among containers, to get the validity rules right.
* A reverse iterator also stores a neighboring iterator. If you erase an element, you often invalidate a reverse iterator that designates the element `before` the one erased. Surprise, surprise.

I figured that it was easier to violate a principle that is already widely violated, particularly if the violation helped me uphold one that is already widely upheld. I hope to start shipping template class `slist` before much longer. Then the C++ community will have another set of options to try out, just as with hash tables.
