#include "doctest.h"
#include "order.hpp"
#include "portfolio.hpp"
#include "order.hpp"      // included twice on purpose
#include "money.hpp"

TEST_CASE("cents converts euros and hundredths") {
  CHECK(cents(Money{12, 34}) == 1234);
  CHECK(cents(Money{0, 5}) == 5);
}

TEST_CASE("notional of an order") {
  Order o{Money{10, 50}, 3};
  CHECK(notional(o) == 3150);
}

TEST_CASE("portfolio value") {
  Portfolio p{Money{100, 0}, 2, Money{25, 25}};
  CHECK(value(p) == 10000 + 5050);
}
