#include "doctest.h"
#include <thread>
#include "bank.hpp"

TEST_CASE("simple transfers") {
  Account a{ 1, 100, {} }, b{ 2, 0, {} };
  CHECK(transfer(a, b, 60));
  CHECK_FALSE(transfer(a, b, 60));
  CHECK_FALSE(transfer(a, a, 10));
  CHECK(a.balance == 40);
  CHECK(b.balance == 60);
}
TEST_CASE("opposite directions concurrently: no deadlock, money conserved") {
  Account a{ 1, 1'000'000, {} }, b{ 2, 1'000'000, {} };
  std::thread t1{ [&] { for (int i = 0; i < 20000; ++i) transfer(a, b, 1); } };
  std::thread t2{ [&] { for (int i = 0; i < 20000; ++i) transfer(b, a, 1); } };
  t1.join();
  t2.join();
  CHECK(a.balance + b.balance == 2'000'000);
  CHECK(a.balance == 1'000'000);
}
