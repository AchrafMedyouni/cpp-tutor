#include <iostream>

int * getVal() {
  static int val{};        // lives for the whole program
  return &val;
}

int next_order_id() {
  static int counter{ 0 }; // initialised once, keeps its value between calls
  return ++counter;
}

int main() {
  std::cout << *getVal() << "\n";
  *getVal() = 8;           // the returned pointer is still valid
  std::cout << *getVal() << "\n";
  std::cout << "next order ids: " << next_order_id() << " " << next_order_id() << " " << next_order_id() << "\n";
}
