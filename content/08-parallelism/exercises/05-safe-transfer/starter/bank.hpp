#pragma once
#include <mutex>

struct Account {
  int id;
  long balance;
  std::mutex m;
};

inline bool transfer(Account& from, Account& to, long amount) {
  std::lock_guard<std::mutex> l1{ from.m };
  std::lock_guard<std::mutex> l2{ to.m };
  if (from.balance < amount) return false;
  from.balance -= amount;
  to.balance += amount;
  return true;
}
