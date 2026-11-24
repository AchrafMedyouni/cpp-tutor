#include "pricing.hpp"

// Definitions: compiled separately into pricing.o
double fee_rate() { return 0.0025; }

double with_fees(double price) { return price * (1.0 + fee_rate()); }
