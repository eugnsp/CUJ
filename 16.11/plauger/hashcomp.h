        // TEMPLATE CLASS hash_compare
template<class _K,
    class _Bop = less<_K> >
    class hash_compare {
public:
    enum {bucket_size = 4,    // 0 < bucket_size
        min_buckets = 8};    // min_buckets = 2 ^^ N, 0 < N
    hash_compare()
        : comp()
        {}
    hash_compare(_Bop _Parg)
        : comp(_Parg)
        {}
    size_t operator()(const _K& _Key) const
        {return (_Key); }
    bool operator()(const _K& _Key1, const _K& _Key2) const
        {return (comp(_Key1, _Key2)); }
private:
    _Bop comp;
    };
