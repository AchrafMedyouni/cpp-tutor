#pragma once
#include <stdexcept>
#include <string>

class Account {
public:
  explicit Account(std::string owner, double initial = 0) : owner_{ std::move(owner) }, balance_{ initial } {}
  const std::string& owner() const { return owner_; }
  double balance() const { return balance_; }
  void deposit(double amount) { balance_ += amount; }        // TODO: validate
  bool withdraw(double amount) { balance_ -= amount; return true; }   // TODO: invariant!
  int operations() const { return 0; }                        // TODO
private:
  std::string owner_;
  double balance_;
};
