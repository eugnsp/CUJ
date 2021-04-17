    typedef _Alloc allocator_type;
    typedef typename _Alloc::size_type size_type;
    typedef typename _Alloc::difference_type _Dift;
    typedef _Dift difference_type;
    typedef typename _Alloc::pointer _Tptr;
    typedef typename _Alloc::const_pointer _Ctptr;
    typedef _Tptr pointer;
    typedef _Ctptr const_pointer;
    typedef typename _Alloc::reference _Reft;
    typedef _Reft reference;
    typedef typename _Alloc::const_reference const_reference;
    typedef typename _Alloc::value_type value_type;

        // CLASS const_iterator
    class const_iterator;
    friend class const_iterator;

    class const_iterator
        : public iterator<forward_iterator_tag, _Ty, _Dift,
            _Ctptr, const_reference>
        {       // iterator for nonmutable slist
    public:
        typedef forward_iterator_tag iterator_category;
        typedef _Ty value_type;
        typedef _Dift difference_type;
        typedef _Ctptr pointer;
        typedef const_reference reference;

        const_iterator()
            : _Pptr(0)
            {}      // construct with null node pointer

        const_iterator(_Nodeptr *_Ppnode)
            : _Pptr(_Ppnode)
            {}      // construct with node pointer pointer 
                    // _Ppnode

        const_reference operator*() const
            {       // return designated value
            return (_Myval(*_Pptr)); }

        _Ctptr operator->() const
            {       // return pointer to class object
            return (&**this); }

        const_iterator& operator++()
            {       // preincrement
            if (_Pptr != 0 && *_Pptr != 0)
                _Pptr = &_Pptr->_Next;
            return (*this); }

        const_iterator operator++(int)
            {       // postincrement
            const_iterator _Tmp = *this;
            ++*this;
            return (_Tmp); }

        bool operator==(const const_iterator& _Right) const
            {       // test for iterator equality
            return (_Pptr == _Right._Pptr); }

        bool operator!=(const const_iterator& _Right) const
            {       // test for iterator inequality
            return (!(*this == _Right)); }

        _Nodeptr *_Mypnode() const
            {       // return node pointer
            return (_Pptr); }

    protected:
        _Nodeptr *_Pptr;    // pointer to pointer to node
        };
