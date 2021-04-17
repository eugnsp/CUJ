    iterator erase(iterator _Where)
        {       // erase element at _Where
        _Erase(_Where._Mypnode());
        return (_Where); }

    iterator erase(iterator _First, iterator _Last)
        {       // erase [_First, _Last)
        for (bool _Done = _First == _Last; !_Done; )
            {       // erase _First, testing _Last before it dies
            if (&_Next(*_First._Mypnode()) == _Last._Mypnode())
                _Done = true;
            erase(_First); }    // _First points to next 
                                // after erase
        return (_First); }

    void _Erase(_Nodeptr *_Ppnode)
        {       // erase element at _Pnode
        if (*_Ppnode != 0)
            {       // not off end, safe to erase
            _Nodeptr _Pnode = _Next(*_Ppnode);
            if (_Pnode == 0)
                _Tail = 0;      // erasing tail, new tail unknown

            _Mybase::_Alval.destroy(&_Myval(*_Ppnode));
            _Freenode(*_Ppnode);
            *_Ppnode = _Pnode;
            --_Mysize; }}


