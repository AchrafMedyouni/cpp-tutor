#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "Tableau.hpp"

TEST_CASE("v2: no leak at the end of the scope") {
  long before = cpplab::live_allocations();
  {
    Tableau t{ 100 };
    t.get(99) = 1;
  }
  CHECK(cpplab::live_allocations() == before);
}
