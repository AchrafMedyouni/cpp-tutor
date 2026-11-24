#include <iostream>

struct Order { int id; double price; };

int main() {
  int processed = 0;
  for (int id = 1; id <= 3; ++id) {
    Order *o = new Order{ id, 100.0 + id };
    if (o->price > 0) ++processed;
    // forgot: delete o;
  }
  std::cout << "processed " << processed << " orders\n";
}
