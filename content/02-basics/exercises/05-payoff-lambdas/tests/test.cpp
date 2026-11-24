#include "doctest.h"
#include "payoffs.hpp"

TEST_CASE("call") {
  Payoff c = make_call(100);
  CHECK(c(90) == 0);
  CHECK(c(130) == doctest::Approx(30));
}
TEST_CASE("put") {
  Payoff p = make_put(100);
  CHECK(p(90) == doctest::Approx(10));
  CHECK(p(130) == 0);
}
TEST_CASE("bull spread is capped") {
  Payoff s = make_bull_spread(100, 110);
  CHECK(s(95) == 0);
  CHECK(s(105) == doctest::Approx(5));
  CHECK(s(200) == doctest::Approx(10));
}
TEST_CASE("combine: straddle and put-call parity at expiry") {
  Payoff straddle = combine(make_call(100), make_put(100), 1, 1);
  CHECK(straddle(80) == doctest::Approx(20));
  CHECK(straddle(120) == doctest::Approx(20));
  Payoff parity = combine(make_call(100), make_put(100), 1, -1);   // C - P = S - K
  CHECK(parity(137) == doctest::Approx(37));
  CHECK(parity(63) == doctest::Approx(-37));
}
TEST_CASE("factories outlive their arguments") {
  Payoff c;
  { double k = 50; c = make_call(k); }
  CHECK(c(60) == doctest::Approx(10));
}
