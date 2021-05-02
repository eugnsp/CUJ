Listing 1 - A simple array class template

// array1.cpp

#include <stddef.h>

template <class T>
class array
    {
public:
    array(size_t n = 0);
    array(const array &a);
    ~array();
    array &operator=(const array &a);
    T &operator[](size_t i);
    size_t length();
private:
    T *pa;
    size_t len;
    };

// ...

template <class T>
T &array<T>::operator[](size_t i)
    {
    return pa[i];
    }

template <class T>
inline size_t array<T>::length()
    {
    return len;
    }

...

template <class T>
T sigma(array<T> &a)
    {
    T sum = 0;
    for (size_t i = 0; i < a.length(); ++i)
        sum += a[i];
    return sum;
    }
