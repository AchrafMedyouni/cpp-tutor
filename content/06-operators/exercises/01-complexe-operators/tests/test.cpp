#include "doctest.h"
#include "Complexe.hpp"

TEST_CASE("compound assignment returns *this") {
  Complexe a{ 1, 2 };
  (a += Complexe{ 1, 1 }) += Complexe{ 1, 1 };
  CHECK(a == Complexe{ 3, 4 });
  a -= Complexe{ 3, 0 };
  CHECK(a == Complexe{ 0, 4 });
}
TEST_CASE("multiplication") {
  Complexe a{ 1, 2 }, b{ 3, -1 };
  CHECK(a * b == Complexe{ 5, 5 });
  a *= a;
  CHECK(a == Complexe{ -3, 4 });   // aliasing: a *= a must still be correct
}
TEST_CASE("binary operators are symmetric") {
  Complexe c{ 1, 1 };
  CHECK(2.0 + c == Complexe{ 3, 1 });
  CHECK(c + 2.0 == Complexe{ 3, 1 });
  CHECK(1.0 - c == Complexe{ 0, -1 });
  CHECK(-c == Complexe{ -1, -1 });
  CHECK(c != Complexe{ 1, 0 });
}
