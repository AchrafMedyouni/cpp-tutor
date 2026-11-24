#pragma once
#include <memory>
#include <string>
#include <vector>

struct Desk;

inline int& destroyed() { static int n = 0; return n; }

struct Trader {
  std::string name;
  std::shared_ptr<Desk> desk;
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

inline std::string Trader::desk_name() const { return desk ? desk->name : "(none)"; }

inline void hire(const std::shared_ptr<Desk>& d, const std::shared_ptr<Trader>& t) {
  d->traders.push_back(t);
  t->desk = d;
}
