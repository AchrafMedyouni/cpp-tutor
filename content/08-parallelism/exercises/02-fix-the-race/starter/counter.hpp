#pragma once
#include <map>
#include <mutex>
#include <string>

class FillCounter {
public:
  void record(const std::string& symbol) { ++fills_[symbol]; }
  int count(const std::string& symbol) const {
    auto it = fills_.find(symbol);
    return it == fills_.end() ? 0 : it->second;
  }
private:
  std::map<std::string, int> fills_;
};
