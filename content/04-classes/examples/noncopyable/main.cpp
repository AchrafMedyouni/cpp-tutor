#include <iostream>

class OrderIdGenerator {
public:
  OrderIdGenerator() = default;
  OrderIdGenerator( const OrderIdGenerator & ) = delete;
  OrderIdGenerator & operator=( const OrderIdGenerator & ) = delete;
  long next() { return ++last_; }
private:
  long last_{ 0 };
};

int main() {
  OrderIdGenerator gen;
  std::cout << gen.next() << " " << gen.next() << "\n";
  OrderIdGenerator copy{ gen };   // error: use of deleted copy constructor
  std::cout << copy.next() << "\n";
}
