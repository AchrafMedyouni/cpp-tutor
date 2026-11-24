#pragma once

class Account {
public:
  explicit Account(double initial) : balance_{ initial } {}
  virtual ~Account() = default;
  double balance() const { return balance_; }
  bool withdraw(double amount) { (void)amount; return false; }   // TODO
protected:
  double balance_;
};

// TODO: Checking, Savings
class Checking : public Account { public: Checking(double initial, double) : Account{ initial } {} };
class Savings : public Account { public: explicit Savings(double initial) : Account{ initial } {} };
