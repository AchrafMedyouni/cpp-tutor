#include "doctest.h"
#include "cpplab/alloc_tracker.h"
#include "desk.hpp"

TEST_CASE("everything is destroyed when the desk goes away") {
  long live = cpplab::live_allocations();
  int before = destroyed();
  {
    auto rates = std::make_shared<Desk>("Rates");
    hire(rates, std::make_shared<Trader>("Alice"));
    hire(rates, std::make_shared<Trader>("Bob"));
    CHECK(rates->traders[0]->desk_name() == "Rates");
  }
  CHECK(destroyed() == before + 3);
  CHECK(cpplab::live_allocations() == live);
}
TEST_CASE("a trader outliving the desk sees (none)") {
  std::shared_ptr<Trader> carol = std::make_shared<Trader>("Carol");
  {
    auto fx = std::make_shared<Desk>("FX");
    hire(fx, carol);
    CHECK(carol->desk_name() == "FX");
  }
  CHECK(carol->desk_name() == "(none)");
}
