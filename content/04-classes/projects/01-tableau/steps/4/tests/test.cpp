#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "Tableau.hpp"

TEST_CASE("v4: assignment copies, frees the old buffer, survives self-assignment") {
  long before = cpplab::live_allocations();
  {
    Tableau a{ 4 }, b{ 2 };
    a.get(3) = 7;
    b = a;
    CHECK(b.taille() == 4);
    CHECK(b.get(3) == 7);
    b.get(3) = -1;
    CHECK(a.get(3) == 7);
    Tableau& alias = b;
    b = alias;
    CHECK(b.get(3) == -1);
    (b = a) = a;   // returns *this by reference
    CHECK(b.get(3) == 7);
  }
  CHECK(cpplab::live_allocations() == before);
}
