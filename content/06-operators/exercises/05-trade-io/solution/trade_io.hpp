#pragma once
#include <iomanip>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

struct Trade { std::string symbol; bool buy; long qty; double price; };

inline std::ostream& operator<<(std::ostream& os, const Trade& t) {
  std::ostringstream px;
  px << std::fixed << std::setprecision(2) << t.price;   // formats a copy: os keeps its own flags
  return os << t.symbol << ' ' << (t.buy ? "BUY" : "SELL") << ' ' << t.qty << " @ " << px.str();
}

inline std::istream& operator>>(std::istream& is, Trade& t) {
  std::string symbol, side, at;
  long qty;
  double price;
  if (!(is >> symbol >> side >> qty >> at >> price)) return is;
  if ((side != "BUY" && side != "SELL") || at != "@") {
    is.setstate(std::ios::failbit);
    return is;
  }
  t = Trade{ symbol, side == "BUY", qty, price };
  return is;
}
