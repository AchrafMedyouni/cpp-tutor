#include "doctest.h"
#include "filter.hpp"

TEST_CASE("counting and summing circles") {
  Cercle a{ 1 }, b{ 2.5 };
  Rectangle r;
  std::vector<const Forme*> v{ &a, &r, &b, &r };
  CHECK(count_cercles(v) == 2);
  CHECK(sum_rayons(v) == doctest::Approx(3.5));
  CHECK(count_cercles({}) == 0);
}
TEST_CASE("is_cercle on references") {
  Cercle a{ 1 };
  Rectangle r;
  CHECK(is_cercle(a));
  CHECK_FALSE(is_cercle(r));
}
