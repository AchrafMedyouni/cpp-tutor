#include <functional>
#include <iostream>
#include <thread>

void f( int & x ) {
  for( int i = 0; i < 1'000'000; ++i ) x += 1;   // load, add, store: not atomic
}

int main() {
  int n{ 0 };
  std::thread t1{ f, std::ref( n ) };
  std::thread t2{ f, std::ref( n ) };
  t1.join();
  t2.join();
  std::cout << "n = " << n << " (expected 2000000)\n";
}
