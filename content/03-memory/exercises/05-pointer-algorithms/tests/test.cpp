#include "doctest.h"
#include "ptr_algos.hpp"

TEST_CASE("reverse odd and even lengths") {
  int a[] = { 1, 2, 3, 4, 5 };
  reverse(a, a + 5);
  CHECK(a[0] == 5); CHECK(a[2] == 3); CHECK(a[4] == 1);
  int b[] = { 1, 2 };
  reverse(b, b + 2);
  CHECK(b[0] == 2); CHECK(b[1] == 1);
  reverse(b, b);   // empty range: nothing happens
  CHECK(b[0] == 2);
}
TEST_CASE("find_first") {
  int a[] = { 4, 7, 7, 1 };
  CHECK(find_first(a, a + 4, 7) == a + 1);
  CHECK(find_first(a, a + 4, 9) == a + 4);
}
TEST_CASE("sum and max_pos") {
  int a[] = { 3, -1, 9, 9, 2 };
  CHECK(sum(a, a + 5) == 22);
  CHECK(max_pos(a, a + 5) == a + 2);
  CHECK(max_pos(a, a) == a);
}
