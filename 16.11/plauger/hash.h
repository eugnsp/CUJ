        // TEMPLATE CLASS _Hash
template<class _Tr>
    class _Hash : public _Tr {
public:
    typedef _Hash<_Tr> _Myt;
    typedef typename _Tr::key_type key_type;
    typedef typename _Tr::key_compare key_compare;
    typedef typename _Tr::value_compare value_compare;
    enum {bucket_size = key_compare::bucket_size,
        min_buckets = key_compare::min_buckets,
        _Multi = _Tr::_Multi};
    typedef list<typename _Tr::value_type,
        typename _Tr::allocator_type> _Mylist;
    typedef typename _Mylist::allocator_type allocator_type;
    typedef typename _Mylist::size_type size_type;
    typedef typename _Mylist::difference_type difference_type;
    typedef typename _Mylist::pointer pointer;
    typedef typename _Mylist::const_pointer const_pointer;
    typedef typename _Mylist::reference reference;
    typedef typename _Mylist::const_reference const_reference;
    typedef typename _Mylist::iterator iterator;
    typedef typename _Mylist::const_iterator const_iterator;
    typedef typename _Mylist::reverse_iterator
        reverse_iterator;
    typedef typename _Mylist::const_reverse_iterator
        const_reverse_iterator;
    typedef typename _Mylist::value_type value_type;
    typedef vector<iterator, allocator_type::template<>
        rebind<iterator>::other> _Myvec;
    typedef pair<iterator, bool> _Pairib;
    typedef pair<iterator, iterator> _Pairii;
    typedef pair<const_iterator, const_iterator> _Paircc;
    explicit _Hash(const key_compare& _Parg,
        const allocator_type& _Al)
        : _Tr(_Parg), _List(_Al), _Vec(min_buckets, end(), _Al),
            _Mask(min_buckets - 1), _Maxidx(min_buckets - 1)
        {}
    _Hash(const value_type *_F, const value_type *_L,
        const key_compare& _Parg, const allocator_type& _Al)
        : _Tr(_Parg), _List(_Al), _Vec(min_buckets, end(), _Al),
            _Mask(min_buckets - 1), _Maxidx(min_buckets - 1)
        {insert(_F, _L); }
    _Hash(const _Myt& _X)
        : _List(_X.get_allocator()), _Vec(_X.get_allocator())
        {_Copy(_X); }
    ~_Hash()
        {}
    _Myt& operator=(const _Myt& _X)
        {if (this != &_X)
            _Copy(_X);
        return (*this); }
    iterator begin()
        {return (_List.begin()); }
    const_iterator begin() const
        {return (_List.begin()); }
    iterator end()
        {return (_List.end()); }
    const_iterator end() const
        {return (_List.end()); }
    reverse_iterator rbegin()
        {return (_List.rbegin()); }
    const_reverse_iterator rbegin() const
        {return (_List.rbegin()); }
    reverse_iterator rend()
        {return (_List.rend()); }
    const_reverse_iterator rend() const
        {return (_List.rend()); }
    size_type size() const
        {return (_List.size()); }
    size_type max_size() const
        {return (_List.max_size()); }
    bool empty() const
        {return (_List.empty()); }
    allocator_type get_allocator() const
        {return (_List.get_allocator()); }
    key_compare key_comp() const
        {return (comp); }
    value_compare value_comp() const
        {return (value_compare(key_comp())); }
    _Pairib insert(const value_type& _V)
        {iterator _P, _Pe;
        if (_Maxidx <= size() / bucket_size)
            {if (_Vec.size() - 1 <= _Maxidx)
                {_Mask = (_Vec.size() << 1) - 1;
                _Vec.resize(_Mask + 1, end()); }
            size_type _N = _Maxidx - (_Vec.size() >> 1);
            for (_P = _Vec[_N], _Pe = _Vec[_N + 1]; _P != _Pe; )
                if ((comp(_Kfn()(*_P)) & _Mask) == _N)
                    ++_P;
                else
                    {for (_N = _Maxidx; _Vec[_N] == end(); --_N)
                        {_Vec[_N] = _P;
                        if (_N == 0)
                            break; }
                    _List.splice(end(), _List, _P++); }
            ++_Maxidx; }
        size_type _N = _Hashval(_Kfn()(_V));
        for (_P = _Vec[_N + 1], _Pe = _Vec[_N]; _P != _Pe; )
            if (comp(_Kfn()(_V), _Kfn()(*--_P)))
                ;
            else if (_Multi)
                break;
            else
                return (_Pairib(_P, false));
        _Pe = _List.insert(_P, _V);
        for (; _P == _Vec[_N]; --_N)
            {_Vec[_N] = _Pe;
            if (_N == 0)
                break; }
        return (_Pairib(_Pe, true)); }
    iterator insert(iterator _P, const value_type& _V)
        {if (!comp(_Kfn()(*_P), _Kfn()(_V))
            && !comp(_Kfn()(_V), _Kfn()(*_P)))
            return (_List.insert(++_P, _V));
        else
            return (insert(_V).first); }
    template<class _It>
        void insert(iterator _F, iterator _L)
        {for (; _F != _L; ++_F)
            insert(*_F); }
    typedef const value_type *_It;
    void insert(_It *_F, _It *_L)
        {for (; _F != _L; ++_F)
            insert(*_F); }
    iterator erase(iterator _P)
        {size_type _N = _Hashval(_Kfn()(*_P));
        for (; _P == _Vec[_N]; --_N)
            {++_Vec[_N];
            if (_N == 0)
                break; }
        return (_List.erase(_P)); }
    iterator erase(iterator _F, iterator _L)
        {if (size() == 0 || _F != begin() || _L != end())
            {while (_F != _L)
                erase(_F++);
            return (_F); }
        else
            {_List.clear();
            _Vec.assign(min_buckets, end());
            _Mask = min_buckets - 1;
            _Maxidx = min_buckets - 1;
            return (end()); }}
    size_type erase(const key_type& _X)
        {_Pairii _P = equal_range(_X);
        size_type _N = 0;
        _Distance(_P.first, _P.second, _N);
        erase(_P.first, _P.second);
        return (_N); }
    void erase(const key_type *_F, const key_type *_L)
        {for (; _F != _L; ++_F)
            erase(*_F); }
    void clear()
        {erase(begin(), end()); }
    iterator find(const key_type& _Kv)
        {return (lower_bound(_Kv)); }
    const_iterator find(const key_type& _Kv) const
        {return (lower_bound(_Kv)); }
    size_type count(const key_type& _Kv) const
        {_Paircc _Ans = equal_range(_Kv);
        size_type _N = 0;
        _Distance(_Ans.first, _Ans.second, _N);
        return (_N); }
    iterator lower_bound(const key_type& _Kv)
        {size_type _N = _Hashval(_Kv);
        iterator _P, _Pe;
        for (_P = _Vec[_N], _Pe = _Vec[_N + 1]; _P != _Pe; ++_P)
            if (!comp(_Kfn()(*_P), _Kv))
                return (comp(_Kv, _Kfn()(*_P)) ? end() : _P);
        return (end()); }
    const_iterator lower_bound(const key_type& _Kv) const
        {size_type _N = _Hashval(_Kv);
        const_iterator _P, _Pe;
        for (_P = _Vec[_N], _Pe = _Vec[_N + 1]; _P != _Pe; ++_P)
            if (!comp(_Kfn()(*_P), _Kv))
                return (comp(_Kv, _Kfn()(*_P)) ? end() : _P);
        return (end()); }
    iterator upper_bound(const key_type& _Kv)
        {size_type _N = _Hashval(_Kv);
        iterator _P, _Pe;
        for (_P = _Vec[_N + 1], _Pe = _Vec[_N]; _P != _Pe; )
            if (!comp(_Kv, _Kfn()(*--_P)))
                return (comp(_Kfn()(*_P), _Kv) ? end() : ++_P);
        return (end()); }
    const_iterator upper_bound(const key_type& _Kv) const
        {size_type _N = _Hashval(_Kv);
        const_iterator _P, _Pe;
        for (_P = _Vec[_N + 1], _Pe = _Vec[_N]; _P != _Pe; )
            if (!comp(_Kv, _Kfn()(*--_P)))
                return (comp(_Kfn()(*_P), _Kv) ? end() : ++_P);
        return (end()); }
    _Pairii equal_range(const key_type& _Kv)
        {size_type _N = _Hashval(_Kv);
        iterator _P, _Pl, _Pe;
        for (_P = _Vec[_N], _Pe = _Vec[_N + 1]; _P != _Pe; ++_P)
            if (!comp(_Kfn()(*_P), _Kv))
                {for (_Pl = _P; _P != _Pe; ++_P)
                    if (comp(_Kv, _Kfn()(*_P)))
                        break;
                if (_Pl == _P)
                    break;
                return (_Pairii(_Pl, _P)); }
        return (_Pairii(end(), end())); }
    _Paircc equal_range(const key_type& _Kv) const
        {size_type _N = _Hashval(_Kv);
        const_iterator _P, _Pl, _Pe;
        for (_P = _Vec[_N], _Pe = _Vec[_N + 1]; _P != _Pe; ++_P)
            if (!comp(_Kfn()(*_P), _Kv))
                {for (_Pl = _P; _P != _Pe; ++_P)
                    if (comp(_Kv, _Kfn()(*_P)))
                        break;
                if (_Pl == _P)
                    break;
                return (_Paircc(_Pl, _P)); }
        return (_Paircc(end(), end())); }
    void swap(_Myt& _X)
        {if (get_allocator() == _X.get_allocator())
            {_List.swap(_X._List);
            _STD swap(_Vec, _X._Vec);
            _STD swap(_Mask, _X._Mask);
            _STD swap(_Maxidx, _X._Maxidx);
            _STD swap(comp, _X.comp); }
        else
            {_Myt _Ts = *this; *this = _X, _X = _Ts; }}
protected:
    void _Copy(const _Myt& _X)
        {_Vec.resize(_X._Vec.size(), end());
        _Mask = _X._Mask;
        _Maxidx = _X._Maxidx;
        _List.clear();
        try
            {_List.insert(end();
            _X._List.begin();
            _X._List.end()); }
        catch (...)
            {fill(_Vec.begin(), _Vec.end(), end());
            throw; }
        iterator _P = begin();
        const_iterator _Pe = _X.begin();
        for (size_type _N = 0; _N < _Vec.size(); )
            if (_Pe == _X._Vec[_N])
                _Vec[_N] = _P, ++_N;
            else
                ++_P, ++_Pe;
        comp = _X.comp; }
    size_type _Hashval(const key_type& _Key) const
        {size_type _N = comp(_Key) & _Mask;
        if (_Maxidx <= _N)
            _N -= _Vec.size() >> 1;
        return (_N); }
    key_compare comp;
    _Mylist _List;    // must precede _Vec
    _Myvec _Vec;
    size_type _Mask, _Maxidx;
    };
