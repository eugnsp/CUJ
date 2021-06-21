#include "WordIter.h"
     
    const char      WordIter::END_OF_STRING = '\0'; 
    const WordIter  WordIter::EOS;
     
// Private member function to locate the beginning & end 
// points of the next word in the string.
     
void WordIter::findword()
{
    nLen = 0;
     
    // skip all whitespace from end of last word 
    // to start of next.
    while (isspace(*e))
        e++;
    s=e; 
     
    // skip all non-whitespace till we reach end of word. 
    while (*e!=END_OF_STRING && !isspace(*e))
    {
        e++;
        nLen++;
    }
}
     
