    iterator insert(iterator _Where, const _Ty& _Val)
        {       // insert _Val at _Where
        _Insert(_Where._Mypnode(), _Val);
        return (_Where); }

    void 
    insert(iterator _Where, size_type _Count, const _Ty& _Val)
        {       // insert _Count * _Val at _Where
        size_type _Countsave = _Count;

        try {
        for (; 0 < _Count; --_Count)
            _Insert(_Where._Mypnode(), _Val);
        } catch {
        for (; _Count < _Countsave; ++_Count)
            _Erase(_Where._Mypnode());      // undo inserts
        throw; }}

    template<class _Iter>
        void insert(iterator _Where, _Iter _First, _Iter _Last)
        {       // insert [_First, _Last) at _Where
        _Insert(_Where, _First, _Last, _Iter_cat(_First)); }

    template<class _Iter>
        void _Insert(iterator _Where, _Iter _Count, _Iter _Val,
            _Int_iterator_tag)
        {       // insert _Count * _Val at _Where
        insert(_Where, (size_type)_Count, (const _Ty&)_Val); }

    template<class _Iter>
        void _Insert(iterator _Where, _Iter _First, _Iter _Last,
            input_iterator_tag)
        {       // insert [_First, _Last) at _Where, 
                // input iterators
        size_type _Num = 0;

        try {
        iterator _Here = _Where;
        for (; _First != _Last; ++_First, ++_Num, ++_Here)
            _Insert(_Here._Mypnode(), *_First);
        } catch {
        for (; 0 < _Num; --_Num)
            _Erase(_Where._Mypnode());      // undo inserts
        throw; }}

    template<class _Iter>
        void _Insert(iterator _Where, _Iter _First, _Iter _Last,
            forward_iterator_tag)
        {       // insert [_First, _Last) at _Where, 
                // forward iterators
        _Iter _Next = _First;

        try {
        iterator _Here = _Where;
        for (; _First != _Last; ++_First, ++_Here)
            _Insert(_Here._Mypnode(), *_First);
        } catch {
        for (; _Next != _First; ++_Next)
            _Erase(_Where._Mypnode());      // undo inserts
        throw; }}

    void _Insert(_Nodeptr *_Ppnode, const _Ty& _Val)
        {       // insert _Val at *_Ppnode
        _Nodeptr _Newnode = 0;

        try {
        _Incsize(1);
        _Newnode = _Buynode(*_Ppnode);
        _Mybase::_Alval.construct(&_Myval(_Newnode), _Val);
        } catch {
        if (_Newnode != 0)
            {       // allocation succeeded, constructor failed
            --_Mysize;
            _Freenode(_Newnode); }
        throw; }

        if (_Next(_Newnode) == 0)
            _Tail = _Newnode;       // new tail
        *_Ppnode = _Newnode; }
