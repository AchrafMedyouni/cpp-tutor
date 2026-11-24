#pragma once
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <mutex>
#include <optional>

template <typename T>
class BlockingQueue {
public:
  explicit BlockingQueue(std::size_t capacity) : cap_{ capacity } {}
  bool push(T value) { (void)value; return false; }       // TODO
  std::optional<T> pop() { return std::nullopt; }          // TODO
  void close() {}                                          // TODO
  std::size_t size() const { return 0; }                   // TODO
private:
  std::size_t cap_;
};
