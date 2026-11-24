#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include <string>
#include "portfolio.hpp"

TEST_CASE("market value and P&L") {
  Portfolio p = make_portfolio(4);
  add_fill(p, "AAPL", 100, 180.0);
  add_fill(p, "MSFT", 10, 400.0);
  add_fill(p, "NVDA", -5, 900.0);   // short
  const std::string syms[] = { "MSFT", "AAPL", "NVDA" };
  const double marks[] = { 410.0, 190.0, 950.0 };
  CHECK(market_value(p, syms, marks, 3) == doctest::Approx(100 * 190.0 + 10 * 410.0 - 5 * 950.0));
  CHECK(market_value(p, syms, marks, 1) == doctest::Approx(4100.0));   // only MSFT marked
  CHECK(unrealized_pnl(p, "AAPL", 190.0) == doctest::Approx(1000.0));
  CHECK(unrealized_pnl(p, "TSLA", 250.0) == 0);
  destroy(p);
}
