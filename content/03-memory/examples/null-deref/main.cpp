#include <iostream>

struct Quote { double price; int size; };

// Returns the first quote with size >= min_size, or nullptr if none.
Quote* find_liquid(Quote* quotes, int n, int min_size) {
  for (int i = 0; i < n; ++i)
    if (quotes[i].size >= min_size) return &quotes[i];
  return nullptr;
}

int main() {
  Quote book[] = { { 100.1, 5 }, { 100.2, 20 } };
  Quote* best = find_liquid(book, 2, 50);   // nobody shows 50
  std::cout << best->price << "\n";         // null dereference
}
