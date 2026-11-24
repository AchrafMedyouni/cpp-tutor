#include <iostream>
#include <stdexcept>

class Account {
public:
  explicit Account(double initial) : balance_{ 0 } { deposit(initial); }

  double balance() const { return balance_; }

  void deposit(double amount) {
    if (amount <= 0) throw std::invalid_argument("deposit must be positive");
    balance_ += amount;
  }
  bool withdraw(double amount) {        // invariant: balance_ >= 0
    if (amount <= 0 || amount > balance_) return false;
    balance_ -= amount;
    return true;
  }

private:
  double balance_;
};

int main() {
  Account acc{ 100 };
  std::cout << "balance: " << acc.balance() << "\n";
  if (!acc.withdraw(250)) std::cout << "withdraw 250 refused\n";
  std::cout << "balance: " << acc.balance() << "\n";
  try {
    acc.deposit(-5);
  } catch (const std::invalid_argument& e) {
    std::cout << "error: " << e.what() << "\n";
  }
}
