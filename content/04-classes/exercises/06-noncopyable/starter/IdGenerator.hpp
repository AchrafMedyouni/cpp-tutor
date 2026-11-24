#pragma once
#include <string>

class IdGenerator {
public:
  explicit IdGenerator(std::string prefix) : prefix_{ std::move(prefix) } {}
  std::string next() { return prefix_; }   // TODO
  long issued() const { return 0; }        // TODO
private:
  std::string prefix_;
};
