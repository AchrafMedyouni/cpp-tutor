#pragma once
#include <map>
#include <mutex>
#include <string>

class FillCounter {
public:
  void record(const std::string& symbol) {
    std::lock_guard<std::mutex> lock{ m_ };
    ++fills_[symbol];
  }
  int count(const std::string& symbol) const {
    std::lock_guard<std::mutex> lock{ m_ };
    auto it = fills_.find(symbol);
    return it == fills_.end() ? 0 : it->second;
  }
private:
  mutable std::mutex m_;   // mutable: locked in const member functions
  std::map<std::string, int> fills_;
};
