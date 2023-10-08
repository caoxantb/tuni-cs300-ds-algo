/**
 * iteration3.cc
 *
 * Print beginning half of a list
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration3.hh"
using namespace std;

void printHalf(const list<int> &lst) {
  auto it = lst.begin();
  
  for (int i = 0; i < lst.size()/2; i++) {
    cout << *it << " ";
    it++;
  }
  cout << endl;
}
