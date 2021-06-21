//
// File: IteratorToMember.h
// 
// Created: Feb 1998
//
// Author: Thomas Becker
//
// Description:
// ============
//
// Declaration of class template CIteratorToMember
//

#ifndef _ITERATOR_TO_MEMBER_INCLUDED
#define _ITERATOR_TO_MEMBER_INCLUDED

///////////////////////////////////////////////////////////////////////
//
// The class template CIteratorToMember is a wrapper around the 
// iterator specified in the template argument _It. There is a private 
// member of type _It. All operations are passed on to this embedded 
// iterator. In the case of dereferencing via *, ->, or [], the 
// iterator to member passes to a member of the value of the
// embedded iterator. This member is specified in the constructor.
// The second template argument must be the value type of the 
// embedded iterator. The third template argument is the type of the
// member that the iterator to member passes to.
//
template<class _It, class _T, class _R>
class CIteratorToMember
{
  
public:
  
  // If you have a compiler and an STL implementation that fully 
  // suppport the iterator_traits mechanism, uncomment the next four
  // lines and comment out the one following them.

  // typedef std::iterator_traits<_It>::iterator_category 
  //   iterator_category;
  // typedef std::iterator_traits<_It>::distance_type 
  //   distance_type;

  typedef ptrdiff_t distance_type;
  
  typedef _R value_type;
  typedef _R* pointer;
  typedef _R& reference;
  
  // Construct a smart iterator from an iterator.
  CIteratorToMember(_It from, _R _T::* memptr) : 
    m_it(from), m_memptr(memptr){}
  
  // Return the embedded original iterator
  _It GetIterator() const;
    
  // The operators *, ->, and [] apply the functional to the value of 
  // the embedded iterator.
  reference operator*();
  pointer operator->() const;
  reference operator[](distance_type _N) const;
  
  // All other operators behave the standard way.
  CIteratorToMember& operator++();
  CIteratorToMember operator++(int);
  CIteratorToMember& operator--();
  CIteratorToMember operator--(int);
  CIteratorToMember& operator+=(distance_type _N);
  CIteratorToMember operator+(distance_type _N) const;
  CIteratorToMember& operator-=(distance_type _N);
  CIteratorToMember operator-(distance_type _N) const;
  
  bool operator==(const CIteratorToMember<_It, _T, _R>& rhs) const
    {return m_it == rhs.m_it;}
  bool operator!=(const CIteratorToMember<_It, _T, _R>& rhs) const
    {return m_it != rhs.m_it;}
  
protected:
  _It m_it;

  _R _T::* m_memptr;
  
  } ;

template<class _It, class _T, class _R>
inline _It CIteratorToMember<_It, _T, _R>::GetIterator() const
  {return m_it;}
    
template<class _It, class _T, class _R>
inline CIteratorToMember<_It, _T, _R>::reference 
CIteratorToMember<_It, _T, _R>::operator*() 
  {return (*m_it).*m_memptr; } 

template<class _It, class _T, class _R>
inline CIteratorToMember<_It, _T, _R>::pointer 
CIteratorToMember<_It, _T, _R>::operator->() const  
  {return &((*m_it).*m_memptr); } 

template<class _It, class _T, class _R>
inline CIteratorToMember<_It, _T, _R>::reference 
CIteratorToMember<_It, _T, _R>::operator[](distance_type _N) const
  {return m_it[_N].*m_memptr; }

template<class _It, class _T, class _R>
inline CIteratorToMember<_It, _T, _R>& 
CIteratorToMember<_It, _T, _R>::operator++() 
  {++m_it; 
   return *this; }

template<class _It, class _T, class _R>
inline CIteratorToMember<_It, _T, _R> 
CIteratorToMember<_It, _T, _R>::operator++(int) 
  {CIteratorToMember<_It, _T, _R> _Tmp = *this;
  ++m_it;
  return _Tmp; }

template<class _It, class _T, class _R>
inline CIteratorToMember<_It, _T, _R>& 
CIteratorToMember<_It, _T, _R>::operator--() 
  {--m_it;  
  return (*this); }

template<class _It, class _T, class _R>
inline CIteratorToMember<_It, _T, _R> 
CIteratorToMember<_It, _T, _R>::operator--(int)
  {CIteratorToMember<_It, _T, _R> _Tmp = *this;
  --m_it;
  return _Tmp;}

template<class _It, class _T, class _R>
inline CIteratorToMember<_It, _T, _R>& 
CIteratorToMember<_It, _T, _R>::operator+=(distance_type _N)
  {m_it += _N;
  return *this; }

template<class _It, class _T, class _R>
inline CIteratorToMember<_It, _T, _R> 
CIteratorToMember<_It, _T, _R>::operator+(distance_type _N) const
  {return CIteratorToMember<_It, _T, _R>(m_it + _N, m_memptr); }

template<class _It, class _T, class _R>
inline CIteratorToMember<_It, _T, _R>& 
CIteratorToMember<_It, _T, _R>::operator-=(distance_type _N)
  {m_it -= _N;
  return *this; }

template<class _It, class _T, class _R>
inline CIteratorToMember<_It, _T, _R> 
CIteratorToMember<_It, _T, _R>::operator-(distance_type _N) const
  {return CIteratorToMember<_It, _T, _R>(m_it - _N, m_memptr); }

///////////////////////////////////////////////////////////////////////
//
// The MakeIteratorToMember() function template generates a
// an iterator to member from an iterator and a pointer to member.
//
template<class _It, class _T, class _R>
CIteratorToMember<_It, _T, _R> 
MakeIteratorToMember(_It it, _R _T::* memptr)
  { return CIteratorToMember<_It, _T, _R>(it, memptr);}

#endif // end multiple inclusion protection
