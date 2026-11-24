#pragma once
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <numeric>
#include <stdexcept>
#include <vector>

template <typename T>
class TimeSeries {
public:
  using value_type = T;
  using iterator = typename std::vector<T>::iterator;
  using const_iterator = typename std::vector<T>::const_iterator;

  TimeSeries() = default;
  TimeSeries(std::initializer_list<T> l) : data_(l) {}
  template <typename It>
  TimeSeries(It first, It last) : data_(first, last) {}

  void push_back(const T& x) { data_.push_back(x); }
  std::size_t size() const { return data_.size(); }
  bool empty() const { return data_.empty(); }
  T& operator[](std::size_t i) { return data_[i]; }
  const T& operator[](std::size_t i) const { return data_[i]; }

  iterator begin() { return data_.begin(); }
  iterator end() { return data_.end(); }
  const_iterator begin() const { return data_.begin(); }
  const_iterator end() const { return data_.end(); }

private:
  std::vector<T> data_;
};
