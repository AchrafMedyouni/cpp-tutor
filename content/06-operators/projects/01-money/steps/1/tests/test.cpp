#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "Money.hpp"

TEST_CASE("arithmetic") {
  Money a{ 1050, "EUR" }, b{ 250, "EUR" };
  CHECK((a + b).cents() == 1300);
  CHECK((a - b).cents() == 800);
  CHECK((3 * b).cents() == 750);
  CHECK((-a).cents() == -1050);
  a += b;
  CHECK(a == Money{ 1300, "EUR" });
}
TEST_CASE("comparisons") {
  CHECK(Money{ 1, "EUR" } < Money{ 2, "EUR" });
  CHECK(Money{ 5, "EUR" } >= Money{ 5, "EUR" });
  CHECK_FALSE(Money{ 5, "EUR" } == Money{ 5, "USD" });
}
TEST_CASE("currency safety") {
  Money e{ 100, "EUR" }, u{ 100, "USD" };
  CHECK_THROWS_AS(e + u, std::invalid_argument);
  CHECK_THROWS_AS((void)(e < u), std::invalid_argument);
}
