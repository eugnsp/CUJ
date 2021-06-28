# Shim Classes

**Douglas Reichard**

*Douglas Reichard lives in Fairview Park, Ohio and is a senior software engineer for Rockwell Automation. He writes programming software for Programable Logic Control devices. He has been working with C++ since 1991. He specializes in Object-Oriented Design and creating reusable code via templates and design patterns. He can be reached at djreichard@ra.rockwell.com.*

---

You don’t always have to rewrite ordinary classes as templates to get the benefits of templatization.

## Introduction

I always feel frustrated when I have to copy and paste source code, and the only modification I’m making is to the data type being manipulated by the code. It never feels right to have to do this. What’s more, it always seems that once I have copied, then pasted the code everywhere, I find some minor flaw in the original code. Then I have to edit each of the copies and fix the flaw. What should be a single block of functionality is pasted numerous times creating multiple maintenance points.

C++ templates and template libraries like the STL (Standard Template Library) offer frustrated programmers like myself a set of reusable classes. You supply the data type and C++’s template instantiation mechanism supplies the code. The great thing about STL and other libraries is that they supply one “cookie cutter” that can be used to make several “cookies”. From a maintenance perspective, this approach provides a single maintenance point when new functionality is added or flaws are found. If the cookie cutter is fixed, all the cookies are fixed as well.

Templates also give you reusable code without losing type safety. Before STL and templates, container classes were often implemented using `void *`; thus they completely stripped the type off of classes. The user of the container class had to cast the `void *` to a data type, with no way to be sure that the cast was valid.

Even with templates I still find myself copying and pasting a lot of redundant code. This is especially true when I use polymorphism and implement virtual functions. Often, when I override a virtual function, the implementation is exactly the same except for the data type of the enclosing class.

Some template libraries such as Microsoft’s ATL (Active Template Library) address these issues with polymorphism by providing templated “cookie cutter” base classes. ATL’s implementation is very specific to the purpose of creating ActiveX components, but the idiom used in the ATL base classes is applicable to just about anything. In this article I refer to this idiom as a “shim class”.

## A Basic Shim Class

A shim class is a template class that is wedged between a base class and its derived classes to provide behavior and/or data that is common to derived classes, but varies in type. The derived classes do not have to reimplement the code over and over again.

```cpp
class ABase
{
protected:
    virtual size_t GetSize() = 0;
    ...
    void UseGetSize()
    {
    ...
    size_t size = GetSize();
    ...
    }
};

class CDerivedOne : public ABase
{
protected:
    virtual size_t GetSize()
    { return sizeof(CDerivedOne); }
};

class CDerivedTwo: public ABase
{
protected:
    virtual size_t GetSize()
    { return sizeof(CDerivedTwo); }
};
```

Figure 1 shows of a set of classes that demonstrate the problem addressed by shim classes. An abstract base class, `ABase`, defines a pure virtual `GetSize` member function. Each concrete derived class must implement this function. The code for both implementations is exactly the same except for the type of the concrete class.

```cpp
class ABase
{
protected:
    virtual size_t GetSize() = 0;
    ...
    void UseGetSize()
    {
    ...
    size_t size = GetSize();
    ...
    }
};

template <class Derived, class Base>
class SGetSize : public Base
{
protected:
    virtual size_t GetSize() { return sizeof(Derived); }
};

class CDerivedOne : public SGetSize<CDerivedOne, ABase> {};
class CDerivedTwo: public SGetSize<CDerivedTwo, ABase> {};
```

Figure 2 introduces the `SGetSize` shim class, which implements the `GetSize` functionality. The `CDerivedOne` and `CDerivedTwo` classes rely on this shim to provide the implementation of `GetSize`. The `SGetSize` shim may not be very useful in itself, but it does illustrate the basic concepts of a shim class.

Shim classes typically have at least two parameters, the base class and a class that is derived from the base class. These are the two classes the shim is wedged between in the inheritance chain.

From an object-oriented design perspective, `CDerivedOne` has an “is-a” relationship to `ABase`. From an implementation perspective, `CDerivedOne` is derived from an instance of the `SGetSize` shim. The shim instance is derived from `ABase`. To clarify the relationship between a concrete class and its base class, you could say that `CDerivedOne` is derived from `ABase` through `SGetSize`.

Since the template parameter makes the shim class aware of the derived class it is being used to help create, it can implement behavior on behalf of that class. Since the base class is a template parameter, it is not bound to a specific class, which makes it flexible enough to handle several forms of inheritance. For example, you could nest multiple shims between the base and derived class ([Figure 3](multshim.h)) or use shims with multiple inheritance ([Figure 4](mishim.h)).

Note that `ABase` should not be made into a template, since `ABase` has additional functionality that does not vary by type. If `ABase` was made into a template it would result in duplication of the code in the `UseGetSize` method. Also, `ABase` serves as a common abstract base class for all the derived types.

If you are concerned that using the shim will contribute to code bloat, remember that the `GetSize` function would have been coded twice anyway. Using shim classes there are still two instances of the code (an instance of the template for each derived class). But there is the added value of a single point of maintenance for the `GetSize` function. Changes to `SGetSize` are reflected in all the derived classes.

## Summary

A shim class is a template class wedged between a base class and its derived classes to provide a reusable implementation that varies only on the derived object’s type or other template parameters. This idiom provides a single point of maintenance for implementing reusable code. Shim classes can be used at any level of an inheritance hierarchy making maintenance and extension of the hierarchy significantly easier.
