#pragma once
#include <cmath>
#include <string>
#include <vector>

struct Market {
  double spot;   // underlying price
  double rate;   // continuously compounded risk-free rate (bonds use it as an annual yield)
  double vol;    // annualised volatility
};

class Instrument {
public:
  explicit Instrument(std::string name) : name_{ std::move(name) } {}
  virtual ~Instrument() = default;
  const std::string& name() const { return name_; }
  virtual double price(const Market& m) const = 0;
private:
  std::string name_;
};

class Bond : public Instrument {
public:
  Bond(std::string name, double face, double coupon, int years)
    : Instrument{ std::move(name) }, face_{ face }, coupon_{ coupon }, years_{ years } {}
  double price(const Market& m) const override {
    double p = 0;
    for (int t = 1; t <= years_; ++t) p += coupon_ * face_ / std::pow(1 + m.rate, t);
    return p + face_ / std::pow(1 + m.rate, years_);
  }
private:
  double face_, coupon_;
  int years_;
};

inline double norm_cdf(double x) { return 0.5 * std::erfc(-x / std::sqrt(2.0)); }

class EuropeanOption : public Instrument {
public:
  EuropeanOption(std::string name, bool is_call, double strike, double maturity)
    : Instrument{ std::move(name) }, call_{ is_call }, k_{ strike }, t_{ maturity } {}
  double price(const Market& m) const override {
    double sq = m.vol * std::sqrt(t_);
    double d1 = (std::log(m.spot / k_) + (m.rate + 0.5 * m.vol * m.vol) * t_) / sq;
    double d2 = d1 - sq;
    double df = std::exp(-m.rate * t_);
    return call_ ? m.spot * norm_cdf(d1) - k_ * df * norm_cdf(d2)
                 : k_ * df * norm_cdf(-d2) - m.spot * norm_cdf(-d1);
  }
private:
  bool call_;
  double k_, t_;
};

class Future : public Instrument {
public:
  Future(std::string name, double maturity) : Instrument{ std::move(name) }, t_{ maturity } {}
  double price(const Market& m) const override { return m.spot * std::exp(m.rate * t_); }
private:
  double t_;
};

class Book {
public:
  Book() = default;
  Book(const Book&) = delete;              // it owns its instruments: no accidental copies
  Book& operator=(const Book&) = delete;
  ~Book() { for (Instrument* i : items_) delete i; }

  void add(Instrument* instrument, double quantity) {   // takes ownership
    items_.push_back(instrument);
    qty_.push_back(quantity);
  }
  std::size_t size() const { return items_.size(); }

  double value(const Market& m) const {
    double v = 0;
    for (std::size_t i = 0; i < items_.size(); ++i) v += qty_[i] * items_[i]->price(m);
    return v;
  }

  double delta(const Market& m, double h = 0.01) const {
    Market up = m, down = m;
    up.spot += h;
    down.spot -= h;
    return (value(up) - value(down)) / (2 * h);
  }

private:
  std::vector<Instrument*> items_;
  std::vector<double> qty_;
};
