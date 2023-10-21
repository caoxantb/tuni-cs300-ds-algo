#include <iterator>
#include <vector>
#include <algorithm>

#include "test.hh" // NOT_FOUND constant

using namespace std;


/**
 * @brief Find the median value of a given vector, whose elements are in random
 *        order. Return NOT_FOUND if the size of the vector is zero.
 *
 * @param v unsorted vector
 * @return int calculated median of parameter vector
 */
int findMedian(std::vector<int>& v) {
  if (v.empty()) return NOT_FOUND;

  std::sort(v.begin(), v.end());
  int size = v.size();
  return size % 2 == 0 ? (v[size/2 - 1] + v[size/2])/2 : v[size/2];
}

