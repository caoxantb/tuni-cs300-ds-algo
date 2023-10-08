/**
 * iteration4.cc
 *
 * Print all items of a list in a reverse order
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration4.hh"
using namespace std;

void printReverse(const list<int> &lst) {
  auto prevEnd = std::prev(lst.end());
  auto prevBegin = std::prev(lst.begin());

  for (auto it = prevEnd; it != prevBegin; it--) {
    cout << *it << " ";
  }
  cout << endl;
}
