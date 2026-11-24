#include "doctest.h"
#include <type_traits>
#include <utility>
#include <vector>
#include "cpplab/alloc_tracker.h"
#include "Tableau.hpp"

TEST_CASE("noexcept move operations") {
  CHECK(std::is_nothrow_move_constructible_v<Tableau>);
  CHECK(std::is_nothrow_move_assignable_v<Tableau>);
}
TEST_CASE("move construction steals the buffer") {
  Tableau a{ 1000 };
  a[0] = 42;
  const double* buf = a.data();
  long allocs = cpplab::total_allocations();
  Tableau b{ std::move(a) };
  CHECK(cpplab::total_allocations() == allocs);   // no allocation
  CHECK(b.data() == buf);                         // same buffer
  CHECK(b[0] == 42);
  CHECK(a.taille() == 0);
}
TEST_CASE("move assignment") {
  long live = cpplab::live_allocations();
  {
    Tableau a{ 10 }, b{ 20 };
    long allocs = cpplab::total_allocations();
    b = std::move(a);
    CHECK(cpplab::total_allocations() == allocs);
    CHECK(b.taille() == 10);
    a = Tableau{ 3 };                             // a moved-from object can be assigned to
    CHECK(a.taille() == 3);
  }
  CHECK(cpplab::live_allocations() == live);
}
