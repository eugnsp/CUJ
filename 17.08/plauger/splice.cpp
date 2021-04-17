void _Splice(iterator _P, _Myt& _X,
    iterator _F, iterator _L,
    size_type _N)    // added argument
    {if (_Mybase::_Alval == _X._Alval)
        {if (this != &_X)
            {_Incsize(_N);        // was updated by caller
            _X._Size -= _N; }    // except when allocators differ
        _Acc::_Next(_Acc::_Prev(_F._Mynode())) =
            _L._Mynode();
        _Acc::_Next(_Acc::_Prev(_L._Mynode())) =
            _P._Mynode();
        _Acc::_Next(_Acc::_Prev(_P._Mynode())) =
            _F._Mynode();
        _Nodeptr _S = _Acc::_Prev(_P._Mynode());
        _Acc::_Prev(_P._Mynode()) =
            _Acc::_Prev(_L._Mynode());
        _Acc::_Prev(_L._Mynode()) =
            _Acc::_Prev(_F._Mynode());
        _Acc::_Prev(_F._Mynode()) = _S; }
    else
        {insert(_P, _F, _L);
        _X.erase(_F, _L); }}
