#pragma once
#include <cmath>
#include <compare>
#include <cstdlib>
#include <iomanip>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

class Money {
public:
  Money(long cents, std::string currency) : cents_{ cents }, ccy_{ std::move(currency) } {}
  long cents() const { return cents_; }
  const std::string& currency() const { return ccy_; }
  // TODO step 1: arithmetic and comparisons
private:
  long cents_;
  std::string ccy_;
};

// TODO step 2: operator<< and operator>>
// TODO step 3: split and apply_bps
