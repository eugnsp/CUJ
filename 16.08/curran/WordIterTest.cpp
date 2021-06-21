#include "WordIter.h"
#include <iostream.h>
#include <vector>
#include <algorithm>
     
     
using std::string;
using std::vector;
using std::copy;
using std::sort;
using std::for_each;
using std::count;
using std::min_element;
     
void PrintString(string & str)
{
    cout << '\"' << str << '\"' << endl;
}
     
     
int main(int argc, char* argv[])
{
    string  test1 = "This is a test of the word iterator.  "
        "It should use a word several times for the word count";
     
    // Explicit use.
    WordIter   wi(test1);
    while ( wi != WordIter())
    {
        cout << '\"' << *wi << '\"' << endl; 
        ++wi;
    }
     
     
    // Using STL algortihms
     
    wi = test1;
    for_each(wi, WordIter(), PrintString);
     
    WordIter pMinWord =  min_element(wi, WordIter()); 
    PrintString(*pMinWord);
     
    cout << count(wi, WordIter::EOS, string("word")) << endl;
     
     
    // caching to a vector for better access. 
    wi = test1;
    vector<string>  sentence;
    copy(wi, WordIter(test1.end()), std::back_inserter(sentence));
     
    for_each(sentence.begin(),sentence.end(), PrintString);
     
    return(0);
}
