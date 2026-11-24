#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>

class PriceRing {
public:
  explicit PriceRing(std::size_t capacity) : capacity_{ capacity }, data_{ new double[capacity] } {}
  void push(double p) { (void)p; }                     // TODO
  std::size_t size() const { return 0; }               // TODO
  std::size_t capacity() const { return capacity_; }
  double at(std::size_t i) const { (void)i; return 0; }   // TODO
  double mean() const { return 0; }                    // TODO
private:
  std::size_t capacity_;
  double* data_;
};
