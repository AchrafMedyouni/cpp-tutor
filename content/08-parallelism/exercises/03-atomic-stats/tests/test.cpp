#include "doctest.h"
#include <thread>
#include <vector>
#include "stats.hpp"

TEST_CASE("single thread") {
  LatencyStats s;
  CHECK(s.mean() == 0);
  s.add(10); s.add(30);
  CHECK(s.count() == 2);
  CHECK(s.mean() == doctest::Approx(20));
  CHECK(s.max() == 30);
}
TEST_CASE("many threads") {
  LatencyStats s;
  std::vector<std::thread> ts;
  for (int t = 0; t < 4; ++t)
    ts.emplace_back([&s, t] { for (long i = 1; i <= 5000; ++i) s.add(i + t * 10000); });
  for (auto& th : ts) th.join();
  CHECK(s.count() == 20000);
  CHECK(s.sum() == 4 * (5000L * 5001 / 2) + 5000L * (0 + 10000 + 20000 + 30000));
  CHECK(s.max() == 35000);
}
