#ifndef MONEY_HPP_INCLUDED
#define MONEY_HPP_INCLUDED

struct Money {
  long units;   // whole euros
  int hundredths;
};

inline long cents(Money m) { return m.units * 100 + m.hundredths; }

#endif
