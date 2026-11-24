#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "Tableau.hpp"

TEST_CASE("v3: copies are independent") {
  long before = cpplab::live_allocations();
  {
    Tableau a{ 3 };
    a.get(0) = 1.5;
    Tableau b{ a };
    CHECK(b.taille() == 3);
    CHECK(b.get(0) == 1.5);
    b.get(0) = 9;
    CHECK(a.get(0) == 1.5);
  }
  CHECK(cpplab::live_allocations() == before);
}
