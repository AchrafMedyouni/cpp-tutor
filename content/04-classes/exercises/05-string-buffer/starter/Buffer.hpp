#pragma once
#include <cstddef>
#include <cstring>
#include <utility>

class Buffer {
public:
  explicit Buffer(const char* s = "") : size_{ std::strlen(s) }, data_{ new char[size_ + 1] } {
    std::memcpy(data_, s, size_ + 1);
  }
  // TODO: destructor, copy constructor, copy assignment
  std::size_t size() const { return size_; }
  const char* c_str() const { return data_; }
  void append(const char* s) { (void)s; }   // TODO
private:
  std::size_t size_;
  char* data_;
};
