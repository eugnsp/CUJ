class balanceIterator 
{
public:
  typedef set<account>::iterator           adapted_type;
  typedef adapted_type::iterator_category  iterator_category;
  typedef adapted_type::value_type         value_type;
  typedef adapted_type::distance_type      difference_type;
  typedef double*                          pointer;
  typedef double&                          reference;

   balanceIterator() {}
   explicit balanceIterator(adapted_type i) :adaptee(i) {}
   template <class Iter>

   adapted_type base() const { return adaptee; }

   reference operator*() const {return const_cast<reference>(adaptee->balance()); }
   pointer  operator->() const { return &(operator*()); }

   balanceIterator& operator++()
   { ++adaptee;
     return (*this); 
   }
   balanceIterator operator++(int)
   { balanceIterator _Tmp = *this;
    ++adaptee;
    return (_Tmp);
   }
   balanceIterator& operator--()
   { --adaptee;
     return (*this); 
   }
   balanceIterator operator--(int)
   { balanceIterator _Tmp = *this;
     --adaptee;
     return (_Tmp);
   }
private:
   adapted_type adaptee;
};

inline bool operator==(const balanceIterator& x, 
                       const balanceIterator& y) {
  return x.base() == y.base();
}
inline bool operator!=(const balanceIterator& x, 
                       const balanceIterator& y) {
  return x.base() != y.base();
}

