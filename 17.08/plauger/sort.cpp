typedef list<_Ty, _Ax> _Myt;
typedef _List_val<_Ty, _Ax> _Mybase;

void sort()
    {if (2 <= size())
        {const size_t _MAXN = 15;
        _Myt _X(_Mybase::_Alval), _Arr[_MAXN + 1];
        size_t _N = 0;
        while (!empty())
            {_X.splice(_X.begin(), *this, begin());
            size_t _I;
            for (_I = 0; _I < _N && !_Arr[_I].empty(); ++_I)
                {_Arr[_I].merge(_X);
                _Arr[_I].swap(_X); }
            if (_I == _MAXN)
//                _Arr[_I].merge(_X);        // not merged at end
                _Arr[_I - 1].merge(_X);
            else
                {_Arr[_I].swap(_X);
                if (_I == _N)
                    ++_N; }}
//        while (0 < _N)        // merged from big to small
//            merge(_A[--_N]); }}
        for (size_t _I = 0; _I < _N; ++_I)
            merge(_Arr[_I]); }}
