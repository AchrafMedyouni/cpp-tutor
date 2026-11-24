#pragma once

class Account {
public:
  explicit Account(double initial) : balance_{ initial } {}
  virtual ~Account() = default;
  double balance() const { return balance_; }
  bool withdraw(double amount) {
    if (amount <= 0 || !can_withdraw(amount)) return false;
    balance_ -= amount;
    on_withdraw();
    return true;
  }
protected:
  virtual bool can_withdraw(double amount) const { return amount <= balance_; }
  virtual void on_withdraw() {}
  double balance_;
};

class Checking : public Account {
public:
  Checking(double initial, double overdraft) : Account{ initial }, overdraft_{ overdraft } {}
protected:
  bool can_withdraw(double amount) const override { return balance_ - amount >= -overdraft_; }
private:
  double overdraft_;
};

class Savings : public Account {
public:
  explicit Savings(double initial) : Account{ initial } {}
protected:
  bool can_withdraw(double amount) const override { return withdrawals_ < 3 && Account::can_withdraw(amount); }
  void on_withdraw() override { ++withdrawals_; }
private:
  int withdrawals_ = 0;
};
