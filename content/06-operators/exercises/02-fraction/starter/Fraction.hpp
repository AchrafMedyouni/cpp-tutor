#pragma once
#include <compare>
#include <numeric>
#include <ostream>
#include <stdexcept>

class Fraction {
public:
  Fraction(long num, long den = 1) : num_{ num }, den_{ den } {}   // TODO: validate, normalise
  long num() const { return num_; }
  long den() const { return den_; }
private:
  long num_, den_;
};

// TODO: + - * /, ==, <=>, <<
