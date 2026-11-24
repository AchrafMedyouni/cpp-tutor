#include "returns.hpp"

#include <cmath>

double simple_return(double p0, double p1) { return p1 / p0 - 1.0; }

double log_return(double p0, double p1) { return std::log(p1 / p0); }

double annualized_vol(double daily_vol) { return daily_vol * std::sqrt(static_cast<double>(trading_days)); }
