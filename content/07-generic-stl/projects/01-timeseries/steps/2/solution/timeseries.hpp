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

template <typename T>
TimeSeries<T> simple_returns(const TimeSeries<T>& p) {
  TimeSeries<T> r;
  if (p.size() < 2) return r;
  std::vector<T> tmp(p.size() - 1);
  std::transform(p.begin() + 1, p.end(), p.begin(), tmp.begin(), [](const T& today, const T& yday) { return today / yday - T(1); });
  return TimeSeries<T>(tmp.begin(), tmp.end());
}

template <typename T>
TimeSeries<T> moving_average(const TimeSeries<T>& x, std::size_t window) {
  if (window == 0) throw std::invalid_argument{ "window must be > 0" };
  TimeSeries<T> out;
  if (x.size() < window) return out;
  T sum = std::accumulate(x.begin(), x.begin() + window, T(0));
  out.push_back(sum / T(window));
  for (std::size_t i = window; i < x.size(); ++i) {
    sum += x[i] - x[i - window];
    out.push_back(sum / T(window));
  }
  return out;
}
