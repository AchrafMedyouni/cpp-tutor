struct Money {
  long units;   // whole euros
  int hundredths;
};

inline long cents(Money m) { return m.units * 100 + m.hundredths; }
