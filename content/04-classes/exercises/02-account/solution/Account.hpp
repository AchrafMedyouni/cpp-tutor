#pragma once
#include <stdexcept>
#include <string>

class Account {
public:
  explicit Account(std::string owner, double initial = 0) : owner_{ std::move(owner) }, balance_{ initial } {
    if (initial < 0) throw std::invalid_argument{ "negative initial balance" };
  }
  const std::string& owner() const { return owner_; }
  double balance() const { return balance_; }
  void deposit(double amount) {
    if (amount <= 0) throw std::invalid_argument{ "deposit must be positive" };
    balance_ += amount;
    ++operations_;
  }
  bool withdraw(double amount) {
    if (amount <= 0 || amount > balance_) return false;
    balance_ -= amount;
    ++operations_;
    return true;
  }
  int operations() const { return operations_; }
private:
  std::string owner_;
  double balance_;
  int operations_ = 0;
};
