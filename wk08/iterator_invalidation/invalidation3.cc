#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * @brief duplicates even numbers in the vector, removes uneven numbers. Example: {1, 2, 3 4} -> {2, 2, 4, 4}
 *
 * @param vec vector to be handled
 */
void duplicateEvenRemoveUneven(std::vector<int> &vec) {
  std::vector<int> result;
  using Iter = std::vector<int>::iterator;

  for (Iter it = vec.begin(); it < vec.end(); it++) {
    if (*it % 2 == 0) {
      result.push_back(*it);
      result.push_back(*it);
    }
  }

  vec = result;
}
