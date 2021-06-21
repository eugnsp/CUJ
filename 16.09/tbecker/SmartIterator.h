//
// File: SmartIterator.h
// 
// Created: Feb 1998
//
// Author: Thomas Becker
//
// Description:
// ============
//
// Declaration of class template CSmartIterator
//

#ifndef _SMART_ITERATOR_INCLUDED
#define _SMART_ITERATOR_INCLUDED

///////////////////////////////////////////////////////////////////////
//
// The class template CSmartIterator
//
template<class _It, class _Func>
class CSmartIterator
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
  
  typedef _Func::result_type value_type;
  
  // Construct a smart iterator from an iterator.
  CSmartIterator(_It from, _Func(func)) : 
    m_it(from), m_func(func)
  {}
  
  // Return the embedded original iterator
  _It GetIterator() const;
    
  // The operators *, and [] apply the functional to the value of 
  // the embedded iterator.
  //
  value_type operator*() const;
  value_type operator[](distance_type _N) const;
  
  // All other operators behave the standard way.
  CSmartIterator& operator++();
  CSmartIterator operator++(int);
  CSmartIterator& operator--();
  CSmartIterator operator--(int);
  CSmartIterator& operator+=(distance_type _N);
  CSmartIterator operator+(distance_type _N) const;
  CSmartIterator& operator-=(distance_type _N);
  CSmartIterator operator-(distance_type _N) const;
  
  bool operator==(const CSmartIterator<_It, _Func>& rhs) const
    {return m_it == rhs.m_it;}
  bool operator!=(const CSmartIterator<_It, _Func>& rhs) const
    {return m_it != rhs.m_it;}

protected:
  _It m_it;
  _Func m_func;
  
} ;


template<class _It, class _Func>
inline _It CSmartIterator<_It, _Func>::GetIterator() const
  {return m_it;}
    
template<class _It, class _Func>
inline CSmartIterator<_It, _Func>::value_type 
CSmartIterator<_It, _Func>::operator*() const
  {return m_func(*m_it); } 

template<class _It, class _Func>
inline CSmartIterator<_It, _Func>::value_type
CSmartIterator<_It, _Func>::operator[](distance_type _N) const
  {return m_func(m_it[_N]); }

template<class _It, class _Func>
inline CSmartIterator<_It, _Func>& 
CSmartIterator<_It, _Func>::operator++() 
  {++m_it; 
   return *this; }

template<class _It, class _Func>
inline CSmartIterator<_It, _Func> 
CSmartIterator<_It, _Func>::operator++(int) 
  {CSmartIterator<_It, _Func> _Tmp = *this;
  ++m_it;
  return _Tmp; }

template<class _It, class _Func>
inline CSmartIterator<_It, _Func>& 
CSmartIterator<_It, _Func>::operator--() 
  {--m_it;  
  return (*this); }

template<class _It, class _Func>
inline CSmartIterator<_It, _Func> 
CSmartIterator<_It, _Func>::operator--(int)
  {CSmartIterator<_It, _Func> _Tmp = *this;
  --m_it;
  return _Tmp;}

template<class _It, class _Func>
inline CSmartIterator<_It, _Func>& 
CSmartIterator<_It, _Func>::operator+=(distance_type _N)
  {m_it += _N;
  return *this; }

template<class _It, class _Func>
inline CSmartIterator<_It, _Func> 
CSmartIterator<_It, _Func>::operator+(distance_type _N) const
  {return CSmartIterator<_It, _Func>(m_it + _N, m_func); }

template<class _It, class _Func>
inline CSmartIterator<_It, _Func>& 
CSmartIterator<_It, _Func>::operator-=(distance_type _N)
  {m_it -= _N;
  return *this; }

template<class _It, class _Func>
inline CSmartIterator<_It, _Func> 
CSmartIterator<_It, _Func>::operator-(distance_type _N) const
  {return CSmartIterator<_It, _Func>(m_it - _N, m_func); }
  
template<class _It, class _Func>
inline CSmartIterator<_It, _Func>
MakeSmartIterator(_It it, _Func func)
  {return CSmartIterator<_It, _Func>(it, func);}
  

#endif // end multiple inclusion protection
