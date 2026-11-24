#include <iomanip>
#include <iostream>
#include <vector>

using Price = double;
using Series = std::vector< Price >;
typedef double Return;            // the C way, same effect

std::vector<Return> simple_returns(const Series& prices) {
  std::vector<Return> r;
  for (std::size_t i = 1; i < prices.size(); ++i)
    r.push_back(prices[i] / prices[i - 1] - 1.0);
  return r;                       // returned by value: the vector's buffer is moved out, not copied
}

int main() {
  Series closes{ 100.0, 101.0, 100.0, 102.0 };
  auto r = simple_returns(closes);
  std::cout << std::fixed << std::setprecision(2) << "returns: ";
  for (Return x : r) std::cout << 100 * x << "% ";
  std::cout << "\ncumulative: " << 100 * (closes.back() / closes.front() - 1) << "%\n";
}
