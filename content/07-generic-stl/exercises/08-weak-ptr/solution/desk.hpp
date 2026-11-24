#pragma once
#include <memory>
#include <string>
#include <vector>

struct Desk;

inline int& destroyed() { static int n = 0; return n; }

struct Trader {
  std::string name;
  std::weak_ptr<Desk> desk;          // observes, does not own
  explicit Trader(std::string n) : name{ std::move(n) } {}
  ~Trader() { ++destroyed(); }
  std::string desk_name() const;
};

struct Desk {
  std::string name;
  std::vector<std::shared_ptr<Trader>> traders;
  explicit Desk(std::string n) : name{ std::move(n) } {}
  ~Desk() { ++destroyed(); }
};

inline std::string Trader::desk_name() const {
  if (auto d = desk.lock()) return d->name;
  return "(none)";
}

inline void hire(const std::shared_ptr<Desk>& d, const std::shared_ptr<Trader>& t) {
  d->traders.push_back(t);
  t->desk = d;
}
