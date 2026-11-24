#pragma once
#include <algorithm>
#include <vector>

class Payoff {
public:
  virtual ~Payoff() = default;
  virtual double operator()(double spot) const = 0;
};

class Call : public Payoff {
public:
  explicit Call(double k) : k_{ k } {}
  double operator()(double s) const override { return std::max(s - k_, 0.0); }
private:
  double k_;
};

class Put : public Payoff {
public:
  explicit Put(double k) : k_{ k } {}
  double operator()(double s) const override { return std::max(k_ - s, 0.0); }
private:
  double k_;
};

class Digital : public Payoff {
public:
  Digital(double k, double cash) : k_{ k }, cash_{ cash } {}
  double operator()(double s) const override { return s > k_ ? cash_ : 0.0; }
private:
  double k_, cash_;
};

class Straddle : public Payoff {
public:
  explicit Straddle(double k) : call_{ k }, put_{ k } {}
  double operator()(double s) const override { return call_(s) + put_(s); }
private:
  Call call_;
  Put put_;
};

inline double expected_payoff(const Payoff& p, const std::vector<double>& scenarios) {
  if (scenarios.empty()) return 0;
  double s = 0;
  for (double x : scenarios) s += p(x);
  return s / scenarios.size();
}
