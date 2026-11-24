#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Payoff {
public:
  virtual ~Payoff() = default;
  virtual double operator()(double spot) const = 0;   // pure virtual
  virtual std::string name() const = 0;
};

class Call : public Payoff {
public:
  explicit Call(double k) : k_{ k } {}
  double operator()(double s) const override { return std::max(s - k_, 0.0); }
  std::string name() const override { return "Call(" + std::to_string(int(k_)) + ")"; }
private:
  double k_;
};

class Put : public Payoff {
public:
  explicit Put(double k) : k_{ k } {}
  double operator()(double s) const override { return std::max(k_ - s, 0.0); }
  std::string name() const override { return "Put(" + std::to_string(int(k_)) + ")"; }
private:
  double k_;
};

class Digital : public Payoff {
public:
  explicit Digital(double k) : k_{ k } {}
  double operator()(double s) const override { return s > k_ ? 1.0 : 0.0; }
  std::string name() const override { return "Digital(" + std::to_string(int(k_)) + ")"; }
private:
  double k_;
};

int main() {
  std::vector<Payoff*> book{ new Call{ 100 }, new Put{ 100 }, new Digital{ 105 } };
  std::cout << std::left << std::setw(6) << "spot";
  for (const Payoff* p : book) std::cout << std::setw(11) << p->name();
  std::cout << "\n" << std::fixed << std::setprecision(2);
  for (double s : { 90.0, 100.0, 110.0 }) {
    std::cout << std::setw(6) << int(s);
    for (const Payoff* p : book) std::cout << std::setw(11) << (*p)(s);
    std::cout << "\n";
  }
  for (Payoff* p : book) delete p;
}
