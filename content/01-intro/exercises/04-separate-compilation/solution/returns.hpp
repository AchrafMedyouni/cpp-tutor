#ifndef RETURNS_HPP_INCLUDED
#define RETURNS_HPP_INCLUDED

constexpr int trading_days = 252;

double simple_return(double p0, double p1);
double log_return(double p0, double p1);
double annualized_vol(double daily_vol);

#endif
