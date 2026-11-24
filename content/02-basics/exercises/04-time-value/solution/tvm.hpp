#pragma once
#include <cmath>

inline double future_value(double pv, double rate, int years, int periods_per_year = 1) {
  return pv * std::pow(1.0 + rate / periods_per_year, years * periods_per_year);
}

inline double present_value(double fv, double rate, int years, int periods_per_year = 1) {
  return fv / std::pow(1.0 + rate / periods_per_year, years * periods_per_year);
}

inline double annuity_payment(double pv, double rate_per_period, int n_periods) {
  if (rate_per_period == 0.0) return pv / n_periods;
  return pv * rate_per_period / (1.0 - std::pow(1.0 + rate_per_period, -n_periods));
}
