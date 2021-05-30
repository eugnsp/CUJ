# Implementing Associative Containers

**P. J. Plauger**

*P. J. Plauger is Senior Editor of **C/C++ Users Journal** and President of Dinkumware, Ltd. He is the author of the Standard C++ Library shipped with Microsoft’s Visual C++, v4.2. For eight years, he served as convener of the ISO C standards committee, WG14. He remains active on the C++ committee, J16. His latest books are **The Draft Standard C++ Library**, **Programming on Purpose** (three volumes), and **Standard C** (with Jim Brodie), all published by Prentice-Hall. You can reach him at pjp@plauger.com.*

---

At the heart of every STL set or map lies a balanced binary tree, or at least a tree that’s almost balanced.

## Introduction

Last month, I introduced the four associative template container classes defined by the Standard Template Library (STL). These include the template classes `set`, `multiset`, `map`, and `multimap`. (See "[Standard C/C++: Associative Containers](../../15.04/plauger/plauger.md)", *CUJ*, April 1997). All are implemented in terms of an underlying template class `Tree`, which stores the controlled sequence.

Template class `Tree` is declared as:

```cpp
template<class K, class Ty, class Kfn, class Pr, class A>
class Tree;
```
where:
* `K` is the type of key values, the values used to order the controlled sequence
* `Ty` is the type of each element of the controlled sequence, which includes the key value
* `Kfn` is a function object type whose member `operator()` returns the key value associated with a given element value
* `Pr` is a function object type whose member `operator()` returns `true` only if the first argument value is ordered before the second argument value
* `A` is an allocator type that can allocate arrays of objects of type `Ty`

For a `set` or `multiset`, the value type `Ty` is the same as the key type `K`. In this case, `Kfn()(X)` simply returns `X`.

For a `map` or `multimap`, the value type is the type `pair<const K, Mty>`, where `Mty` is the type of object stored alongside the key object in each element of the controlled sequence. In this case, `Kfn()(X)` returns `X.first`, the first element of the stored pair of objects.

As you might guess from its name, template class `Tree` represents its controlled sequence as a tree data structure. More specifically, it is a binary tree. Each element node stores, in addition to the element value of type `Ty`, pointers to the parent node and to left and right subtrees.

Even more specifically, it is an ordered binary tree. The left subtree for the node with value `B` contains only elements with values `A` such that `!Pr()(B, A)`. The right subtree contains only elements with values `C` such that `Pr()(B, C)`. For the default predicate, which is the function object type less declared in `<functional>`,this means that the leftmost element of the tree is the smallest. Or to be even more precise, since a tree can contain elements with equivalent ordering, no element in the
tree is ordered before the leftmost element.

As I discussed last month, it is important that a tree remain in balance. The whole idea is to ensure that the number of nodes on the path from the root to any element of the tree be proportional to the logarithm of `N`, the total number of elements in the tree. If a tree can grow long and skinny, it starts looking like a linear list, and it loses its highly desirable search properties.

For a perfectly balanced tree, every left subtree for a node should have just as many nodes as the right subtree, plus or minus one. The logarithmic path length is then guaranteed. Unfortunately, algorithms that maintain perfect balance during insertions and erasures are neither simple nor cheap. True, they work in logarithmic time, but the still take plenty of it, relatively speaking.

Relative to what, you might ask? It has long been an important branch of computer science to determine the best ways to represent large databases. Sorting and searching algorithms have occupied many of the best minds in our business. It makes sense to capture the most appropriate technology in the design of template class Tree. That`s what STL is about, after all.

## Color Me Red/Black

It turns out that the best way to keep a tree more or less in balance is to allow a bit of slack. A tree that is almost balanced is easier to build by insertions and trim by erasures, yet it still maintains good time complexity for searches. The particular flavor of less-than-perfectly-balanced trees used here is called a “red-black tree”. It is not a simple data structure to maintain, by any means, but it has a number of virtues.

Among its virtues, a red-black tree supports insertions, erasures, and lookups in logarithmic time. As important, it does all these operations reasonably fast. Probably more important, its worst-case behavior doesn’t deviate much from its best behavior, no matter how a tree evolves over time. For a container class intended for general use, bounded worst-case behavior is usually more important than the promise of fastest possible performance – so long as that performance is good enough.

The name “red-black tree” derives from the simple coloring algorithm used to keep track of the degree of imbalance in a particular subtree. For an excellent thumbnail description of how such trees work, see Mark Nelson’s handy book on STL [1]. What follows is an even briefer sketch of red-black trees.

One way to allow a limited amount of imbalance is to permit fatter nodes. You might let each node hold one to three elements, instead of just one. Inserting a new element often just fattens an existing node. Only when it is already at the fattest permissible, holding three elements, do you have to do more work. Then you split the node into two parts, pushing the displaced middle element up a level and merging the inserted element into one of the remaining two slimmed-down nodes. You merge the displaced middle element at the next level up by a recursive application of the same rules.

Erasing an element is similar. If it is in a fattened node, the node just gets a bit slimmer. Otherwise, you have to rob an element from one of the subtrees to make a replacement node. Cleaning up after the robbery is once again a recursive application of the same rules for erasing elements.

Fat nodes are a good idea in principle, but they make for messy data structures. Either you have multiple flavors of nodes, or some nodes can have unused elements and pointers. It is in many ways more graceful to represent fat nodes as little binary subtrees of one, two, or three nodes.

Here’s where the coloring trick comes in. Paint black all the links between fat nodes. Paint red all the links inside each node. (Actually, the paint resides within the node pointed at, as a Boolean flag value or an enumeration object.) Then you can tell just by inspecting colors in the immediate neighborhood just where you are in the tree structure.

When looking up an element, you can ignore the colors of links. All you see is an ordered binary tree, which is actually easier to climb over than the tree with fat nodes described above. Insertions and erasures are just as messy, however. They are arguably even harder to understand, with all the tests for red and black links in
place of references to explicit elements inside a fat node. But the algorithms are fast enough, once you get them right.

## Implementing Red-Black Trees

[Listing 1](TREE.H) shows the template class `Tree`. It is an STL container that represents its ordered sequence as a red-black tree. It defines bidirectional iterators that visit the elements of the controlled sequence in order. Unlike the containers I’ve described earlier – `vector`, `list`, and `deque` – it also defines a number of member functions that take advantage of the ordered tree structure. These let you look up elements by key value for fast searches, insertions, and erasures.


The member functions `Dec` and `Inc`, defined in the member classes `iterator` and `const_iterator`, perform the magic of walking the sequence in proper order. If you can understand them, you can understand a lot about how the tree structure enforces the ordering requirements.

Every constructor has a Boolean argument `Marg`, which specifies whether multiple elements can have equivalent ordering. `Marg` is `false` for a `Tree` constructed by `set` or `map`. It is `true` for a `Tree` constructed by `multiset` or `multimap`. Note that none of the constructors, or template arguments for that matter, have defaults. Template class `Tree` is intended for internal use by other STL containers. It does not need all the shorthand conveniences of a user-level template class.

The hard work of erasing an element is encapsulated in the first member function `Erase`. If you want to understand how it works, I recommend that you set aside plenty of time and scratch paper. You have to draw a lot of subtrees to convince yourself that all possible cases are handled properly.

Similarly, the hard work of inserting an element between two adjacent elements is encapsulated in the member function `Insert`. The same caveats apply as for understanding `Erase`, above.

Template class `Tree` has plenty of other mysteries, to be sure. It is a difficult class to design, to write, and to understand. But most of the remaining complexities can also be found in the other STL container classes I have presented over the past several months. I resist the urge to repeat earlier sermons, however much they might help explain this complex template class.

If you enjoy learning clever algorithms, however, I encourage you to study [Listing 1](TREE.H) with care. I certainly enjoyed learning how to write it, by studying the STL code from Hewlett-Packard with care.

## Reference

[1] Mark Nelson. C++ Programmers Guide to the Standard Template Library (IDG Books, Foster City CA, 1995).
