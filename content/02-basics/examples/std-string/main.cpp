#include <iostream>
#include <string>

int main() {
  std::string s{ "Hello" };
  std::string full = s + " world!";                    // concatenation
  std::cout << full << " (" << full.size() << " chars)\n";
  std::cout << "equal: " << (full == "Hello world!") << "\n";   // compares contents

  std::string pair{ "EUR/USD" };
  auto slash = pair.find('/');
  std::string base = pair.substr(0, slash), quote = pair.substr(slash + 1);
  std::string ticker = base + quote;
  std::cout << "ticker: " << ticker << ", base: " << base << ", quote: " << quote << "\n";
  std::cout << "found '/' at " << slash << "\n";
  std::cout << "less: " << (std::string{ "AAPL" } < std::string{ "MSFT" }) << "\n";   // lexicographic
}
