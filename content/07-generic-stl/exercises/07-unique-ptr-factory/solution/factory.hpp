#pragma once
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "instruments.hpp"

inline std::unique_ptr<Instrument> make_instrument(const std::string& spec) {
  std::istringstream in{ spec };
  std::string kind;
  if (!(in >> kind)) return nullptr;
  if (kind == "BOND") {
    double face, coupon;
    if (in >> face >> coupon) return std::make_unique<Bond>(face, coupon);
  } else if (kind == "EQUITY") {
    double last;
    if (in >> last) return std::make_unique<Equity>(last);
  } else if (kind == "TRACKED") {
    double last;
    if (in >> last) return std::make_unique<Tracked>(last);
  }
  return nullptr;
}

inline double book_value(const std::vector<std::unique_ptr<Instrument>>& book) {
  double v = 0;
  for (const auto& i : book) v += i->price();
  return v;
}

inline std::unique_ptr<Instrument> take_largest(std::vector<std::unique_ptr<Instrument>>& book) {
  if (book.empty()) return nullptr;
  auto best = book.begin();
  for (auto it = book.begin(); it != book.end(); ++it)
    if ((*it)->price() > (*best)->price()) best = it;
  std::unique_ptr<Instrument> out = std::move(*best);
  book.erase(best);
  return out;
}
