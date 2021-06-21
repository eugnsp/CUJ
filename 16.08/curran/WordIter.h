#if !defined(AFX_WORDITER_H_INCLUDED_) 
#define AFX_WORDITER_H_INCLUDED_
     
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
     
#include <ctype.h>
#include <iterator>
#include <string>
     
class WordIter : public std::iterator<std::input_iterator_tag,
                     std::string>
     
{
private:
    static const char END_OF_STRING;
     
    char const *s;
    char const *e;
    int        nLen;
     
    void findword();
     
public:
    static const WordIter   EOS;
     
    // Default ctor equal 
    // beyond-the-end.
    WordIter() : s(&END_OF_STRING), e(&END_OF_STRING), nLen(0) {};
     
    WordIter(char const * const S) : e(S) {findword();} 
    WordIter(const std::string S) : e(S.c_str()) {findword();}
     
    WordIter& operator=(char const * const S) 
        {e=S; findword(); return(*this);}
     
    std::string operator*() const           
        {return std::string(s, nLen);}
     
    bool operator==(WordIter rhs) const
    {
        bool retv = false;
        if (*s == *rhs.s)
            if ( (*s==END_OF_STRING) || (s == rhs.s) )
                retv = true;
        return (retv);
    }
    bool operator!=(WordIter rhs) const 
        {return ! operator==(rhs); }
     
    WordIter &operator++()    
        {findword();  return (*this);}
     
    WordIter  operator++(int) 
        {WordIter ret(*this);findword();return (ret);}
};
     
#endif // !defined(AFX_WORDITER_H_INCLUDED_)
