#include "doctest.h"
#include "collatz.hpp"

TEST_CASE("n = 1 needs no step") { CHECK(collatz_steps(1) == 0); }
TEST_CASE("small values") {
  CHECK(collatz_steps(2) == 1);
  CHECK(collatz_steps(6) == 8);
  CHECK(collatz_steps(7) == 16);
}
TEST_CASE("27 takes 111 steps") { CHECK(collatz_steps(27) == 111); }
TEST_CASE("large start (needs 64-bit)") { CHECK(collatz_steps(837799) == 524); }
