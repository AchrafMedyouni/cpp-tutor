#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

int str_to_int( std::string s, int base = 10 ) {
  return std::stoi(s, nullptr, base);
}

// Future value of `pv` after `years`, compounded `periods_per_year` times a year.
double future_value(double pv, double rate, int years, int periods_per_year = 1) {
  return pv * std::pow(1.0 + rate / periods_per_year, years * periods_per_year);
}

int main() {
  int dix  = str_to_int( "10" );
  int deux = str_to_int( "10", 2 );
  int seize = str_to_int( "10", 16 );
  std::cout << "dix = " << dix << ", deux = " << deux << ", seize = " << seize << "\n";
  std::cout << std::fixed << std::setprecision(2);
  std::cout << "FV annual:  " << future_value(1000, 0.05, 10) << "\n";
  std::cout << "FV monthly: " << future_value(1000, 0.05, 10, 12) << "\n";
}
