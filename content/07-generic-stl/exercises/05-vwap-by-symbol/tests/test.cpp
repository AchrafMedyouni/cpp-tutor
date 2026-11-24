#include "doctest.h"
#include "vwap.hpp"

const std::vector<Fill> fills{
  { "AAPL", 100, 190.0 }, { "MSFT", 50, 410.0 }, { "AAPL", 300, 186.0 },
  { "NVDA", 50, 950.0 }, { "MSFT", 150, 412.0 }, { "TSLA", 10, 250.0 } };

TEST_CASE("vwap per symbol") {
  auto v = vwap_by_symbol(fills);
  CHECK(v.size() == 4);
  CHECK(v["AAPL"] == doctest::Approx(187.0));
  CHECK(v["MSFT"] == doctest::Approx(411.5));
  CHECK(v.begin()->first == "AAPL");   // sorted keys
}
TEST_CASE("most traded") {
  CHECK(most_traded(fills, 2) == std::vector<std::string>{ "AAPL", "MSFT" });
  CHECK(most_traded(fills, 10).size() == 4);
  CHECK(most_traded({ { "B", 5, 1 }, { "A", 5, 1 } }, 2) == std::vector<std::string>{ "A", "B" });
}
