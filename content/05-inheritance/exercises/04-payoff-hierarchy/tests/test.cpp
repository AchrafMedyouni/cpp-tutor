#include "doctest.h"
#include <memory>
#include <type_traits>
#include "payoff.hpp"

TEST_CASE("Payoff is abstract") { CHECK(std::is_abstract_v<Payoff>); }
TEST_CASE("individual payoffs") {
  CHECK(Call{ 100 }(120) == doctest::Approx(20));
  CHECK(Put{ 100 }(120) == 0);
  CHECK(Digital{ 100, 5 }(100.01) == doctest::Approx(5));
  CHECK(Digital{ 100, 5 }(100) == 0);
  CHECK(Straddle{ 100 }(90) == doctest::Approx(10));
}
TEST_CASE("expected payoff through a base reference") {
  std::vector<double> s{ 80, 100, 120 };
  CHECK(expected_payoff(Call{ 100 }, s) == doctest::Approx(20.0 / 3));
  CHECK(expected_payoff(Straddle{ 100 }, s) == doctest::Approx(40.0 / 3));
  CHECK(expected_payoff(Put{ 100 }, {}) == 0);
}
TEST_CASE("delete through a base pointer") {
  std::unique_ptr<Payoff> p = std::make_unique<Straddle>(50);
  CHECK((*p)(40) == doctest::Approx(10));
}
