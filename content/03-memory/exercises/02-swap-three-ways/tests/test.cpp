#include "doctest.h"
#include "swaps.hpp"

TEST_CASE("swap_ptr") {
  int x = 1, y = 2;
  swap_ptr(&x, &y);
  CHECK(x == 2);
  CHECK(y == 1);
}
TEST_CASE("swap_ref") {
  int x = 1, y = 2;
  swap_ref(x, y);
  CHECK(x == 2);
  CHECK(y == 1);
}
TEST_CASE("rotate3") {
  int a = 1, b = 2, c = 3;
  rotate3(a, b, c);
  CHECK(a == 2);
  CHECK(b == 3);
  CHECK(c == 1);
}
TEST_CASE("apply_fill") {
  long pos = 0;
  double cash = 1000;
  apply_fill(pos, cash, 10, 50);
  CHECK(pos == 10);
  CHECK(cash == doctest::Approx(500));
  apply_fill(pos, cash, -4, 60);
  CHECK(pos == 6);
  CHECK(cash == doctest::Approx(740));
}
