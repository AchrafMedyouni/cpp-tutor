#include "doctest.h"
#include <thread>
#include <vector>
#include "counter.hpp"

TEST_CASE("concurrent recording") {
  FillCounter c;
  const char* symbols[] = { "AAPL", "MSFT", "NVDA", "TSLA" };
  std::vector<std::thread> ts;
  for (int t = 0; t < 4; ++t)
    ts.emplace_back([&c, &symbols, t] {
      for (int i = 0; i < 2000; ++i) c.record(symbols[(i + t) % 4]);
    });
  for (auto& th : ts) th.join();
  for (const char* s : symbols) CHECK(c.count(s) == 2000);
  CHECK(c.count("IBM") == 0);
}
