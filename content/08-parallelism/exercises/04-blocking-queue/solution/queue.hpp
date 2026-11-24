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

  bool push(T value) {
    std::unique_lock<std::mutex> lock{ m_ };
    not_full_.wait(lock, [&] { return q_.size() < cap_ || closed_; });
    if (closed_) return false;
    q_.push_back(std::move(value));
    not_empty_.notify_one();
    return true;
  }

  std::optional<T> pop() {
    std::unique_lock<std::mutex> lock{ m_ };
    not_empty_.wait(lock, [&] { return !q_.empty() || closed_; });
    if (q_.empty()) return std::nullopt;
    T v = std::move(q_.front());
    q_.pop_front();
    not_full_.notify_one();
    return v;
  }

  void close() {
    std::lock_guard<std::mutex> lock{ m_ };
    closed_ = true;
    not_full_.notify_all();
    not_empty_.notify_all();
  }

  std::size_t size() const {
    std::lock_guard<std::mutex> lock{ m_ };
    return q_.size();
  }

private:
  mutable std::mutex m_;
  std::condition_variable not_full_, not_empty_;
  std::deque<T> q_;
  std::size_t cap_;
  bool closed_ = false;
};
