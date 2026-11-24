#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "Money.hpp"

TEST_CASE("split never loses a cent") {
  auto parts = split(Money{ 10000, "EUR" }, 3);
  REQUIRE(parts.size() == 3);
  CHECK(parts[0].cents() == 3334);
  CHECK(parts[1].cents() == 3333);
  Money total{ 0, "EUR" };
  for (const Money& p : parts) total += p;
  CHECK(total == Money{ 10000, "EUR" });
  auto neg = split(Money{ -5, "USD" }, 2);
  CHECK(neg[0].cents() + neg[1].cents() == -5);
  CHECK_THROWS_AS(split(Money{ 1, "EUR" }, 0), std::invalid_argument);
}
TEST_CASE("basis-point fees") {
  CHECK(apply_bps(Money{ 1000000, "EUR" }, 2.5).cents() == 250);   // 2.5 bp of 10,000 EUR = 2.50 EUR
  CHECK(apply_bps(Money{ 1001, "EUR" }, 50).cents() == 5);          // 5.005 cents rounds to 5
  CHECK(apply_bps(Money{ 1000, "EUR" }, 5).cents() == 1);           // exactly 0.5 cent: half away from zero → 1
  CHECK(apply_bps(Money{ -1000, "EUR" }, 5).cents() == -1);         // and -0.5 → -1
  CHECK(apply_bps(Money{ 300, "EUR" }, 5000).cents() == 150);
}
