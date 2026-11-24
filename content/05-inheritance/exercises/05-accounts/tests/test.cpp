#include "doctest.h"
#include "accounts.hpp"

TEST_CASE("plain account") {
  Account a{ 100 };
  CHECK(a.withdraw(60));
  CHECK_FALSE(a.withdraw(60));
  CHECK(a.balance() == 40);
}
TEST_CASE("checking allows an overdraft") {
  Checking c{ 100, 50 };
  Account& a = c;
  CHECK(a.withdraw(140));
  CHECK(a.balance() == -40);
  CHECK_FALSE(a.withdraw(20));
  CHECK(a.withdraw(10));
}
TEST_CASE("savings: no overdraft, max 3 withdrawals") {
  Savings s{ 100 };
  Account& a = s;
  CHECK_FALSE(a.withdraw(200));
  CHECK(a.withdraw(10));
  CHECK(a.withdraw(10));
  CHECK(a.withdraw(10));
  CHECK_FALSE(a.withdraw(10));
  CHECK(a.balance() == 70);
}
