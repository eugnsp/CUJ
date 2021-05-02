Listing 2 - A simple const-correct array class template

// array2.cpp

#include <stddef.h>

template <class T>
class array
    {
public:
    array(size_t n = 0);
    array(const array &a);
    ~array();
    array &operator=(const array &a);
    const T &operator[](size_t i) const;
    T &operator[](size_t i);
    size_t length() const;
private:
    T *pa;
    size_t len;
    };

// ...

template <class T>
const T &array<T>::operator[](size_t i) const
    {
    return pa[i];
    }

template <class T>
T &array<T>::operator[](size_t i)
    {
    return pa[i];
    }

template <class T>
inline size_t array<T>::length() const
    {
    return len;
    }

...

template <class T>
T sigma(const array<T> &a)
    {
    T sum = 0;
    for (size_t i = 0; i < a.length(); ++i)
        sum += a[i];
    return sum;
    }
