#pragma once
#include <string>

class IdGenerator {
public:
  explicit IdGenerator(std::string prefix) : prefix_{ std::move(prefix) } {}
  IdGenerator(const IdGenerator&) = delete;
  IdGenerator& operator=(const IdGenerator&) = delete;

  std::string next() { return prefix_ + std::to_string(++counter_); }
  long issued() const { return counter_; }

private:
  std::string prefix_;
  long counter_ = 0;
};
