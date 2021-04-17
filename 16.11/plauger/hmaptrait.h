        // TEMPLATE CLASS _Hmap_traits
template<class _K, class _Ty, class _Pr, class _Ax, bool _Mfl>
    class _Hmap_traits {
public:
    typedef _K key_type;
    typedef pair<const _K, _Ty> value_type;
    typedef _Pr key_compare;
    typedef _Ax::template<> rebind<value_type>::other
        allocator_type;
    enum {_Multi = _Mfl};
    _Hmap_traits()
        : comp()
        {}
    _Hmap_traits(_Pr _Parg)
        : comp(_Parg)
        {}
    class value_compare
        : public binary_function<value_type, value_type, bool> {
        friend class _Hmap_traits<_K, _Ty, _Pr, _Ax, _Mfl>;
    public:
        bool operator()(const value_type& _X,
            const value_type& _Y) const
            {return (comp(_X.first, _Y.first)); }
        value_compare(key_compare _Pred)
            : comp(_Pred) {}
    protected:
        key_compare comp;
        };
    struct _Kfn {
        const _K& operator()(const value_type& _X) const
            {return (_X.first); }
        };
    _Pr comp;
    };
