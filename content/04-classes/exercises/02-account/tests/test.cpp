#include "doctest.h"
#include <type_traits>
#include "Account.hpp"

TEST_CASE("construction") {
  const Account a{ "Ada", 100 };
  CHECK(a.owner() == "Ada");
  CHECK(a.balance() == 100);
  CHECK_THROWS_AS(Account("Bob", -1), std::invalid_argument);
  CHECK_FALSE(std::is_convertible_v<std::string, Account>);   // explicit
}
TEST_CASE("deposit") {
  Account a{ "Ada" };
  a.deposit(50);
  CHECK(a.balance() == 50);
  CHECK_THROWS_AS(a.deposit(0), std::invalid_argument);
  CHECK_THROWS_AS(a.deposit(-5), std::invalid_argument);
  CHECK(a.balance() == 50);
}
TEST_CASE("withdraw keeps the invariant") {
  Account a{ "Ada", 100 };
  CHECK(a.withdraw(30));
  CHECK_FALSE(a.withdraw(500));
  CHECK_FALSE(a.withdraw(-1));
  CHECK(a.balance() == 70);
}
TEST_CASE("only successful operations count") {
  Account a{ "Ada", 10 };
  a.deposit(5);
  a.withdraw(100);
  a.withdraw(5);
  CHECK(a.operations() == 2);
}
