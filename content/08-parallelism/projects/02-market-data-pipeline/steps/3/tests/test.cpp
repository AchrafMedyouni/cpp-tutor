#include "doctest.h"
#include "pipeline.hpp"

TEST_CASE("crossovers on a single symbol") {
  SmaCrossStrategy s{ 2, 4 };
  for (double p : { 10.0, 10.0, 10.0, 9.0, 8.0, 12.0, 14.0, 9.0, 7.0 }) s.on_tick({ "X", p, 1 });
  std::vector<Signal> expected{ { "X", true, 12.0 }, { "X", false, 7.0 } };
  CHECK(s.signals() == expected);
}
TEST_CASE("symbols are independent") {
  SmaCrossStrategy s{ 1, 2 };
  for (double p : { 1.0, 2.0, 1.0 }) { s.on_tick({ "A", p, 1 }); s.on_tick({ "B", 10 - p, 1 }); }
  REQUIRE(s.signals().size() == 2);
  CHECK(s.signals()[0] == Signal{ "A", false, 1.0 });
  CHECK(s.signals()[1] == Signal{ "B", true, 9.0 });
}
TEST_CASE("through the pipeline") {
  std::vector<std::string> lines;
  for (int i = 0; i < 60; ++i) lines.push_back("EURUSD," + std::to_string(1.0 + 0.01 * ((i / 10) % 2 ? 10 - i % 10 : i % 10)) + ",1");
  SmaCrossStrategy s{ 2, 5 };
  auto stats = run_pipeline(lines, [&s](const Tick& t) { s.on_tick(t); }, 4);
  CHECK(stats.received == 60);
  CHECK(s.signals().size() >= 4);
}
