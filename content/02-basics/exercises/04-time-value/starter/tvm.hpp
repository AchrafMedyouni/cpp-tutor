#pragma once
#include <cmath>

inline double future_value(double pv, double rate, int years, int periods_per_year = 1) {
  (void)rate; (void)years; (void)periods_per_year;
  return pv;   // TODO
}

inline double present_value(double fv, double rate, int years, int periods_per_year = 1) {
  (void)rate; (void)years; (void)periods_per_year;
  return fv;   // TODO
}

inline double annuity_payment(double pv, double rate_per_period, int n_periods) {
  (void)pv; (void)rate_per_period; (void)n_periods;
  return 0;    // TODO
}
