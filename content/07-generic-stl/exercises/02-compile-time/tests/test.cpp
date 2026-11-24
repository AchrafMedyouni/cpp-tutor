#include "doctest.h"
#include "meta.hpp"

TEST_CASE("Fib template") {
  CHECK(Fib<0>::value == 0);
  CHECK(Fib<1>::value == 1);
  CHECK(Fib<10>::value == 55);
  CHECK(Fib<90>::value == 2880067194370816120ULL);
}
TEST_CASE("constexpr fib") {
  constexpr auto f = fib(50);   // must be computable at compile time
  CHECK(f == 12586269025ULL);
  CHECK(fib(1) == 1);
}
TEST_CASE("Power") {
  CHECK(Power<2, 10>::value == 1024);
  CHECK(Power<10, 0>::value == 1);
  CHECK(Power<3, 4>::value == 81);
}
