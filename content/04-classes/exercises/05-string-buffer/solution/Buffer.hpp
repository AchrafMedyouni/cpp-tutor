#pragma once
#include <cstddef>
#include <cstring>
#include <utility>

class Buffer {
public:
  explicit Buffer(const char* s = "") : size_{ std::strlen(s) }, data_{ new char[size_ + 1] } {
    std::memcpy(data_, s, size_ + 1);
  }
  ~Buffer() { delete[] data_; }
  Buffer(const Buffer& b) : size_{ b.size_ }, data_{ new char[b.size_ + 1] } {
    std::memcpy(data_, b.data_, size_ + 1);
  }
  Buffer& operator=(const Buffer& b) {
    Buffer tmp{ b };
    swap(tmp);
    return *this;
  }
  void swap(Buffer& b) noexcept {
    std::swap(size_, b.size_);
    std::swap(data_, b.data_);
  }

  std::size_t size() const { return size_; }
  const char* c_str() const { return data_; }

  void append(const char* s) {
    std::size_t extra = std::strlen(s);
    char* bigger = new char[size_ + extra + 1];
    std::memcpy(bigger, data_, size_);
    std::memcpy(bigger + size_, s, extra + 1);
    delete[] data_;
    data_ = bigger;
    size_ += extra;
  }

private:
  std::size_t size_;
  char* data_;
};
