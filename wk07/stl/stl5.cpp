#include <iterator>
#include <vector>
#include <algorithm>

#include "test.hh"

using namespace std;


/**
 * @brief Arrange vector in three subsequent sections:
 *        - those divisible by three (asc order)
 *        - those whose reminder is 1 (asc order)
 *        - those whose reminder is 2 (asc order)
 * @param v vector to be sorted
 * @return int EXIT_SUCCESS if everything went OK, EXIT_FAILURE otherwise
 */
int sortMod3(std::vector<int>& v) {
    auto itRemainZero = std::partition(v.begin(), v.end(), [](int val) {
      return val % 3 == 0;
    });
    auto itRemainOne = std::partition(itRemainZero, v.end(), [](int val) {
      return val % 3 == 1;
    });

    std::sort(v.begin(), itRemainZero);
    std::sort(itRemainZero, itRemainOne);
    std::sort(itRemainOne, v.end());

    return EXIT_SUCCESS;
}

