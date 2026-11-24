#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>

class PriceRing {
public:
  explicit PriceRing(std::size_t capacity) : capacity_{ capacity }, data_{ nullptr } {
    if (capacity == 0) throw std::invalid_argument{ "capacity must be > 0" };
    data_ = new double[capacity];
  }
  ~PriceRing() { delete[] data_; }
  PriceRing(const PriceRing& r) : capacity_{ r.capacity_ }, count_{ r.count_ }, head_{ r.head_ }, data_{ new double[r.capacity_] } {
    for (std::size_t i = 0; i < capacity_; ++i) data_[i] = r.data_[i];
  }
  PriceRing& operator=(const PriceRing& r) {
    PriceRing tmp{ r };
    std::swap(capacity_, tmp.capacity_);
    std::swap(count_, tmp.count_);
    std::swap(head_, tmp.head_);
    std::swap(data_, tmp.data_);
    return *this;
  }

  void push(double p) {
    data_[head_] = p;
    head_ = (head_ + 1) % capacity_;
    if (count_ < capacity_) ++count_;
  }
  std::size_t size() const { return count_; }
  std::size_t capacity() const { return capacity_; }
  double at(std::size_t i) const {
    if (i >= count_) throw std::out_of_range{ "PriceRing::at" };
    std::size_t oldest = (head_ + capacity_ - count_) % capacity_;
    return data_[(oldest + i) % capacity_];
  }
  double mean() const {
    if (count_ == 0) return 0;
    double s = 0;
    for (std::size_t i = 0; i < count_; ++i) s += at(i);
    return s / count_;
  }

private:
  std::size_t capacity_;
  std::size_t count_ = 0;
  std::size_t head_ = 0;
  double* data_;
};
