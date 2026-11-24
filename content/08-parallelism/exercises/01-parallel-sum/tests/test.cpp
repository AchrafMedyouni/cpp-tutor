#include "doctest.h"
#include <numeric>
#include "psum.hpp"

TEST_CASE("matches std::accumulate") {
  std::vector<int> v(100'003);
  std::iota(v.begin(), v.end(), -50'000);
  long long expected = std::accumulate(v.begin(), v.end(), 0LL);
  for (unsigned k : { 1u, 2u, 3u, 8u })
    CHECK(parallel_sum(v, k) == expected);
}
TEST_CASE("edge cases") {
  CHECK(parallel_sum({}, 4) == 0);
  CHECK(parallel_sum({ 5 }, 8) == 5);   // more threads than elements
  CHECK(parallel_sum({ 1, 2 }, 0) == 3);
}
