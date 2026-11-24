#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "Tableau.hpp"

TEST_CASE("v1: get returns a reference, with bounds checking") {
  Tableau t{ 5 };
  CHECK(t.taille() == 5);
  CHECK(t.get(4) == 0);
  for (unsigned i = 0; i < 5; ++i) t.get(i) = i * 2.0;
  CHECK(t.get(3) == 6);
  CHECK_THROWS_AS(t.get(5), std::out_of_range);
}
