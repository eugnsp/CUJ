template<class _It, class _Func>
inline CSmartIterator<_It, _Func>
MakeSmartIterator(_It it, _Func func)
  {return CSmartIterator<_It, _Func>(it, func);}
