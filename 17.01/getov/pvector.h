////////////////// PVector.h ///////////////// rado //
//                                                  //
//  Copyright (c) 1998 Radoslav Getov               //
//                                                  //
//  Permission to use, copy, modify and distribute  //
//  this software is granted without fee, provided  //
//  that the above copyright notice and this        //
//  permission notice appear in the modified source //
//  and the source files that #include it.          //
//                                                  //
////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////
//                                                  //
//  Defines template class pvector<>.               //
//                                                  //
//  It is STL vector<> specialization with iterators//
//  persistent on insert operations, and            //
//  deallocating memory on erase operation.         //
//                                                  //
//  The interface is fully compatible with          //
//  std::vector                                     //
//                                                  //
//////////////////////////////////////////////////////


#ifndef PVECTOR_H
#define PVECTOR_H

#include <vector>

// -------------- template pvector<> -----------
template <class T, class A = std::allocator<T> >
class pvector : public std::vector <T, A> 
{
typedef std::vector<T> _BaseType;
typedef pvector<T>     _ThisType;

public:

// -------------- pvector::const_iterator -------
class const_iterator 
   : public std::iterator < 
               std::random_access_iterator_tag, T >
   {
   protected:
   // -------------- data members ---------------
   size_t           _itsInd;  // Index in its vector.
   const _ThisType* _itsVect; // Ptr to its vector.

   const_iterator (const _ThisType * v, size_t index)
      : _itsVect (v), _itsInd (index) {}

   _BaseType::const_iterator _getBaseConstIt() const
      {return((_BaseType*)_itsVect)->begin()+_itsInd;}

   friend pvector <T>;

   public:
   // -------------- ctors ----------------------
   const_iterator ()
      : _itsInd (0), _itsVect (0) {}

   // -------------- dereferencing --------------
   const_reference operator *() const
      { return (*_itsVect)[_itsInd]; }

   const T* operator ->() const
      { return & operator *(); }

   // -------------- iterator arithmetic --------
   #define DEFINE_ARITHMETIC(It)                     \
      /* binary */                                   \
      It operator + (int s) const                    \
         { return It (_itsVect, _itsInd + s); }      \
      It operator - (int s) const                    \
         { return It (_itsVect, _itsInd - s); }      \
      int operator - (It to) const                   \
         { return _itsInd - to._itsInd; }            \
      /* increment */                                \
      It& operator += (int s)                        \
         { _itsInd += s; return *this; }             \
      It& operator ++ ()                             \
         { _itsInd++; return *this; }                \
      It operator ++ (int)                           \
         { It t (*this); _itsInd++; return t; }      \
      /* decrement */                                \
      It& operator -= (int s)                        \
         { _itsInd -= s; return *this; }             \
      It& operator -- ()                             \
         { _itsInd -- ; return *this; }              \
      It  operator -- (int)                          \
         { It t (*this); _itsInd--; return t; }      \


   DEFINE_ARITHMETIC (const_iterator)

   // -------------- iterator comparisons -----------
   bool operator != (const const_iterator & to) const
      { return _itsInd  != to._itsInd 
            || _itsVect != to._itsVect; }

   bool operator == (const const_iterator & to) const
      { return _itsInd == to._itsInd 
            && _itsVect == to._itsVect; } 

   bool operator < (const const_iterator & to) const
      { return _itsInd  < to._itsInd  
            || _itsInd == to._itsInd 
            && _itsVect < to._itsVect; }

   bool operator <= (const const_iterator & to) const
       { return _itsInd  < to._itsInd 
             || _itsInd == to._itsInd 
             && _itsVect <= to._itsVect; }

   bool operator >= (const const_iterator & to) const
      { return !(*this < to); }

   bool operator > (const const_iterator & to) const
      { return (to < *this); }
   }; // -------------- class const_iterator ----

// ------------- class pvector<>::iterator ----------
class iterator : public const_iterator
   {
   // --------------- private ctor --------------
   iterator (const _ThisType* v, size_t index)
      : const_iterator (v, index) {};

   _BaseType::iterator _getBaseIt() const
     {return ((_BaseType*)_itsVect)->begin()+_itsInd;}

   friend pvector <T>;
   
   public:
   // -------------- ctor ----------------------
   iterator () : const_iterator() {}

   // -------------- dereferencing --------------
   reference operator *() const
      { return (*(_BaseType*)_itsVect)[_itsInd]; }
                               
   T* operator->() const      
      { return &this->operator *();}

   // ------------ arithmetic -------------------
   DEFINE_ARITHMETIC (iterator);
   }; // pvector<>::class const_iterator

#undef DEFINE_ARITHMETIC // cleanup 

// -------------- constructors ------------------
explicit pvector (const A& a = A())
   : std::vector <T> (a) {}

explicit pvector (size_type n, 
                  const T& v = T(), 
                  const A& a = A())
   : std::vector <T> (n, v, a) {}

template <class InIt>   
pvector (InIt first, InIt last, const A& a = A())
   : std::vector <T> (a) 
   { for (; first != last; first++) 
        push_back (*first); }

pvector (const pvector& x)
   : std::vector <T> (x) {}

// -------------- iterator returns --------------
iterator begin() 
   { return iterator (this, 0); }
iterator end() 
   { return iterator (this, size());}

const_iterator begin() const
   { return const_iterator (this, 0);  }
const_iterator end() const
   { return const_iterator (this, size()); }

// -------------- reverse iterators -------------
typedef std::reverse_iterator <iterator, T>
   reverse_iterator;
reverse_iterator rbegin() 
   { return (reverse_iterator (end())); }
reverse_iterator rend() 
   { return (reverse_iterator(begin())); }

typedef std::reverse_iterator <const_iterator, T>
   const_reverse_iterator;
const_reverse_iterator rbegin() const
   { return (const_reverse_iterator (end())); }
const_reverse_iterator rend() const
   { return (const_reverse_iterator (begin())); }

// -------------- assign ------------------------
pvector & operator = (const pvector & from) 
   { (_BaseType &) *this = from; return *this; }

// -------------- erases ------------------------
iterator erase (iterator it)
   { 
   _BaseType::erase (it._getBaseIt());
   if (2 * size() < capacity()) 
      {  // deallocate some storage
      pvector copy (*this);
      swap (copy);
      }
   return it; 
   }

void pop_back ()
   { erase (end() - 1); }

iterator erase (iterator from, iterator to)
   { 
   _BaseType::erase (from._getBaseIt(),
                     to.  _getBaseIt());
   if (2 * size() < capacity())
      {
      pvector copy (*this);
      swap (copy);
      }
   return from; 
   }
                 
void clear ()
   { erase (begin(), end()); }

// -------------- inserts -----------------------
iterator insert (iterator it, const T& x = T())
   { _BaseType::insert (it._getBaseIt(), x);
      return it;  }
   
void insert (iterator it, size_type n, const T& x)
   { _BaseType::insert (it._getBaseIt(), n, x); }

template <class InIt>    
void insert (iterator it, InIt first, InIt last)
   { for (; first != last; it++, first++)
        insert (it, *first);  }

}; // ------------ template pvector -------------

#else
#  error Multiple #included file!
#endif
