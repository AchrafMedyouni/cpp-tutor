#include "doctest.h"
#include "positions.hpp"

const Position book[] = { { 10, 5.0 }, { 2, 100.0 }, { 30, 1.0 } };

TEST_CASE("total over a const array") { CHECK(total(book, 3) == doctest::Approx(280)); }
TEST_CASE("notional of a temporary") { CHECK(notional(Position{ 3, 2.5 }) == doctest::Approx(7.5)); }
TEST_CASE("best returns a pointer into the const array") {
  const Position* b = best(book, 3);
  CHECK(b == &book[1]);
}
TEST_CASE("scale modifies the caller's position") {
  Position p{ 100, 1.0 };
  scale(p, 0.5);
  CHECK(p.qty == 50);
}
