# Smart Iterators and STL

**Thomas Becker**

*Thomas Becker works as a senior software engineer for Zephyr Associates, Inc. in Zephyr Cove, NV. He can be reached at thomas@styleadvisor.com.*

---

Because they’re templates, you can smuggle added functionality into STL algorithms all sorts of ways, even through iterators.

## Introduction

Many of the algorithms in the C++ Standard Template Library (STL) come in two versions, a plain one and one that uses a predicate. Take for example the algorithm `find`. In its plain version, it looks for a given element in a container:

```cpp
it = find(it1, it2, elem);
```

Here, `it1` and `it2` are iterators that define a sequence, possibly stored in a container, and `elem` is an object of the same type as the elements of the sequence. The algorithm returns an iterator to the first position in the sequence between `it1` and `it2` – including `it1` but not `it2` – where the element of the sequence equals `elem`. If no such element exists, the function returns `it2`.

Now assume that the elements of the sequence are complex structures, such as a customer-information structure:

```cpp
typedef struct _customer_info_tag {
    std::string str_name;
    long l_id;
    double dbl_balance;
} CUSTOMER_INFO, *LPCUSTOMER_INFO ;
```

It is rather unlikely that you will ever be searching your sequence for a `CUSTOMER_INFO` element that you know beforehand. Rather, you will be looking for an element whose `str_name` member or whose `l_id` member you know. You can’t do this with the plain algorithm `find`. Instead, you must use the related algorithm `find_if`.

To this end, you first write a suitable predicate in the form of a “functional” – a simple class that may be applied to a `CUSTOMER_INFO` object (via its defined `operator()`) and that returns true if the `str_name` member equals a specified name. [Figure 1](matchtst.cpp) shows how to write such a predicate. Now you can search the container for an element whose `str_name` member equals a given customer name:

```cpp
it = find_if(it1, it2, pred("John Doe"));
```

Next, assume that you want to find the minimum of the balances of all the customers in your container. STL has an algorithm that finds the least of all elements in a container between two given iterators:

```cpp
it = min_element(it1, it2);
```

Obviously, this is not what we want here. What we would need is a second version of `min_element` that relates to `min_element` in a similar way as `find_if` relates to `find`:

```cpp
it = min_element_with_func(it1, it2, func);
```

Rather than finding the least of the elements of the container, this algorithm would apply the functional `func` to each element of the container and find the minimum of the return values. In our situation, we would define the `func operator()` as:

```cpp
double func::operator() (CUSTOMER_INFO arg) const
    { return arg.dbl_balance; }
```

Many conceivable uses exist for this idiom. For example, containers often store pointers or auto-pointers to objects. It is rather unlikely that you want to apply an algorithm to the pointers. It is the objects themselves that algorithms need to operate on. It is easy enough to write a functional that dereferences pointers or auto-pointers. If all algorithms came in a version that takes a functional as an additional argument, you could pass this dereferencing functional and thus make the algorithm operate on the objects rather than the pointers. But alas, STL does not in general offer this idiom. Algorithms such as `min_element` do not come in two versions.

## Smart Iterators

However, there is an alternate solution. Rather than having a second version of each algorithm, we may define a generic mechanism to turn a given iterator into a new iterator for the same container such that the value of the new iterator is the result of applying a functional to the value of the old iterator. I call these iterators “smart iterators”, because they relate to plain iterators much like smart pointers relate to pointers. The smart iterator contains the original iterator as a private member. When the smart iterator is dereferenced, it gets the value of its embedded original iterator, applies the functional, and returns the result.

[Figure 2](SmartIterator.h) shows the template class `CSmartIterator`. The template arguments are the type of the underlying iterator and of the functional. Constructor arguments for the smart iterator are the underlying iterator object and the functional object.

Going back to our original example, let us see how we can now find the minimum of all customer balances in a sequence. For the example, let us assume that the sequence is the contents of a `list` object named `customer_list`. (This assumes that you have defined `operator==`, `operator!=`, `operator<`, and `operator>` for `CUSTOMER_INFO`.) First, we need a functional that returns from a `CUSTOMER_INFO` object the `dbl_balance` member. This is shown in [Figure 3](balance.cpp). Note that the member function `operator()` of the functional `Balance` must be declared as a `const` member function, because the function call is made inside `const` member functions of the smart iterator such as `CSmartIterator::operator*`.

We can now define smart iterators at the beginning and the end of the list as follows:

```cpp
CSmartIterator<std::list<CUSTOMER_INFO>::iterator, Balance>
    sit1(
         customer_list.begin(),
         Balance()
         );

CSmartIterator<std::list<CUSTOMER_INFO>::iterator, Balance>
    sit2(
         customer_list.end(),
         Balance()
         );
```

The expression:

```cpp
std::min_element(sit1, sit2)
```

will now return a smart iterator to the customer with the minimal balance. The value of the minimal balance is:

```cpp
*std::min_element(sit1, sit2)
```

If we need not only the minmal balance, but other information about the customer with the minimal balance as well, we may retrieve the original iterator from the smart iterator:

```cpp
std::list<CUSTOMER_INFO>::iterator iter =
    std::min_element(sit1, sit2).GetIterator();
```

`iter` points to the `CUSTOMER_INFO` object for the customer with the minimal balance.

## Smart Iterator Functions

An obvious drawback of the code above is that it makes a program hard to read because of the proliferation of template arguments. Contrary to widespread belief, STL advocates the use of global functions to ameliorate this problem. [Figure 4](MakeSmartIter.cpp) shows the definition of template function `MakeSmartIterator`, which takes an iterator and a functional as arguments and returns a corresponding smart iterator. The function does nothing but construct the smart iterator into its return value. The point here is that C++ template functions deduce their template arguments from their arguments, thus sparing us the trouble of worrying about template arguments altogether. Finding the the minimal balance now requires no more than this call:

```cpp
std::min_element(
    MakeSmartIterator(
        customer_list.begin(),
        Balance()
        ),
    MakeSmartIterator(
        customer_list.end(),
        Balance()
        )
    );
```

The class `CSmartIterator` is very general, allowing any functional to be applied to the iterator value. If you are content with the special case of the example, where the iterator value is a class and you just want to pass to a member of this class, then the template class `CIteratorToMember` of [Figure 5](IteratorToMember.h) is for you. Using `CIteratorToMember`, you don’t need to define a functional at all. The constructor arguments are the original iterator and a pointer to the class member that the new iterator should return. The minimal balance is now given by:

```cpp
std::min_element(
    MakeIteratorToMember(
        customer_list.begin(),
        &CUSTOMER_LIST::dbl_balance
        ),
    MakeIteratorToMember(
        customer_list.end(),
        &CUSTOMER_LIST::dbl_balance
        )
    );
```

`CIteratorToMember` is written so that dereferencing it returns references to the member in question. Hence, iterators to members may be used to modify the members they refer to. If `ittm` is an iterator to member, then `*ittm` is actually an lvalue.

There is much to be said for using inheritance rather than embedding to obtain smart iterators from iterators. However, iterators are often implemented as plain pointers and can thus not serve as base classes. This is one of those situations where C++ is limited by its C heritage. Basic types are not classes, so with abstract concepts such as iterators we simply do not know if they will be classes or not.
