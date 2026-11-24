#include "doctest.h"
#include <type_traits>
#include "IdGenerator.hpp"

TEST_CASE("ids") {
  IdGenerator g{ "ORD-" };
  CHECK(g.next() == "ORD-1");
  CHECK(g.next() == "ORD-2");
  CHECK(g.issued() == 2);
}
TEST_CASE("not copyable") {
  CHECK_FALSE(std::is_copy_constructible_v<IdGenerator>);
  CHECK_FALSE(std::is_copy_assignable_v<IdGenerator>);
}
