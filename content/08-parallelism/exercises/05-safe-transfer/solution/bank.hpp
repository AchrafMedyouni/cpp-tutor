#pragma once
#include <mutex>

struct Account {
  int id;
  long balance;
  std::mutex m;
};

inline bool transfer(Account& from, Account& to, long amount) {
  if (&from == &to) return false;
  std::scoped_lock lock{ from.m, to.m };   // locks both, deadlock-free
  if (from.balance < amount) return false;
  from.balance -= amount;
  to.balance += amount;
  return true;
}
