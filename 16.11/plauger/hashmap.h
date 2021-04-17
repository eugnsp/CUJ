        // TEMPLATE CLASS hash_map
template<class _K, class _Ty,
    class _Pr = hash_compare<_K, less<_K> >,
    class _A = allocator<pair<const _K, _Ty> > >
    class hash_map
        : public _Hash<_Hmap_traits<_K, _Ty, _Pr, _A, false> > {
public:
    typedef hash_map<_K, _Ty, _Pr, _A> _Myt;
    typedef _Hash<_Hmap_traits<_K, _Ty, _Pr, _A, false> >
        _Mybase;
    typedef _K key_type;
    typedef _Ty mapped_type;
    typedef _Ty referent_type;
    typedef _Pr key_compare;
    typedef typename _Mybase::value_compare value_compare;
    typedef typename _Mybase::allocator_type allocator_type;
    typedef typename _Mybase::size_type size_type;
    typedef typename _Mybase::difference_type difference_type;
    typedef typename _Mybase::pointer pointer;
    typedef typename _Mybase::const_pointer const_pointer;
    typedef typename _Mybase::reference reference;
    typedef typename _Mybase::const_reference const_reference;
    typedef typename _Mybase::iterator iterator;
    typedef typename _Mybase::const_iterator const_iterator;
    typedef typename _Mybase::reverse_iterator reverse_iterator;
    typedef typename _Mybase::const_reverse_iterator
        const_reverse_iterator;
    typedef typename _Mybase::value_type value_type;
    hash_map()
        : _Mybase(key_compare(), allocator_type()) {}
    explicit hash_map(const key_compare& _Pred)
        : _Mybase(_Pred, allocator_type()) {}
    hash_map(const key_compare& _Pred, const allocator_type& _Al)
        : _Mybase(_Pred, _Al) {}
    template<class _It>
        hash_map(_It _F, _It _L)
        : _Mybase(key_compare(), allocator_type())
        {for (; _F != _L; ++_F)
            insert(*_F); }
    template<class _It>
        hash_map(_It _F, _It _L, const key_compare& _Pred)
        : _Mybase(_Pred, allocator_type())
        {for (; _F != _L; ++_F)
            insert(*_F); }
    template<class _It>
        hash_map(_It _F, _It _L, const key_compare& _Pred,
        const allocator_type& _Al)
        : _Mybase(_Pred, _Al)
        {for (; _F != _L; ++_F)
            insert(*_F); }
    mapped_type& operator[](const key_type& _Kv)
        {iterator _P =
            insert(value_type(_Kv, mapped_type())).first;
        return ((*_P).second); }
    };
