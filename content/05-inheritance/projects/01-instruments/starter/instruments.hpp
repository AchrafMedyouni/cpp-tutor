#pragma once
#include <cmath>
#include <string>
#include <vector>

struct Market {
  double spot;   // underlying price
  double rate;   // continuously compounded risk-free rate (bonds use it as an annual yield)
  double vol;    // annualised volatility
};

// Step 1: abstract Instrument + Bond
// Step 2: EuropeanOption (Black–Scholes)
// Step 3: Future + Book
