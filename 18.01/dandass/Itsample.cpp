#pragma warning(disable:4786)
#include <limits.h>
#include <iostream>
#include "observation.h"
#include "itree.h"

#define MAX_OBS       1000
#define QUERY_VALUE   1.0

int main()
{
  itree<observation> Tree;    // The interval tree

  // Add some intervals to the tree
  for (short s = 0; s < MAX_OBS; ++s) {
    time_t                  tmTime;
    observation::range_type rHigh, rLow;

    rLow = (float)rand() / RAND_MAX * 70.0;
    rHigh = rLow + (float)rand() / RAND_MAX * 40.0;
    time(&tmTime);

    observation obs(rHigh, rLow, s, tmTime);
    Tree.push_back(obs);
  }

  // Put the tree in query mode
  itree<observation> const &ConstTree = Tree.construct();

  // Use the const iterator to access all intervals sequentially
  std::cout << "Brute-force results\n";
  itree<observation>::const_iterator citer;
  for(citer = ConstTree.begin(); 
      citer != ConstTree.end(); ++citer)
    if ((citer->low() <= QUERY_VALUE) && 
        (citer->high() >= QUERY_VALUE))
      std::cout << "ID: " << citer->id()
                << ", Low: " << citer->low() 
                << ", High: " << citer->high() << std::endl;

  // Use the query iterator to access intervals containing the
  // value 1.0
  std::cout << "Query results\n";
  itree<observation>::query_iterator iter;
  for (iter = Tree.qbegin(QUERY_VALUE); 
       iter != Tree.qend(QUERY_VALUE); ++iter)
    std::cout << "ID: " << iter->id() 
              << ", Low: " << iter->low() 
              << ", High: " << iter->high() << std::endl;

  return 0;
}
