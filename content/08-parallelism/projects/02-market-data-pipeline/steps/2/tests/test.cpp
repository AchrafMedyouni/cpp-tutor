#include "doctest.h"
#include "pipeline.hpp"

namespace {
std::vector<std::string> feed(int n) {
  std::vector<std::string> v;
  for (int i = 0; i < n; ++i) {
    v.push_back("SYM" + std::to_string(i % 3) + "," + std::to_string(100 + i) + ",10");
    if (i % 10 == 0) v.push_back("garbage line");
  }
  return v;
}
}

TEST_CASE("every valid tick arrives, in order") {
  std::vector<double> prices;
  auto stats = run_pipeline(feed(500), [&prices](const Tick& t) { prices.push_back(t.price); }, 1);
  CHECK(stats.received == 500);
  CHECK(stats.rejected == 50);
  REQUIRE(prices.size() == 500);
  bool ordered = true;
  for (std::size_t i = 1; i < prices.size(); ++i) ordered = ordered && prices[i] > prices[i - 1];
  CHECK(ordered);
}
TEST_CASE("empty feed") {
  int calls = 0;
  auto stats = run_pipeline({}, [&calls](const Tick&) { ++calls; });
  CHECK(stats.received == 0);
  CHECK(calls == 0);
}
