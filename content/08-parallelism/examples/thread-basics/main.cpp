#include <functional>
#include <iostream>
#include <thread>

void f1( int n )   { n = 100; (void)n; }   // receives a copy: the caller's n1 is unaffected
void f2( int & n ) { n = 0; }         // receives a reference (needs std::ref)
struct F3 {
  bool * ran;
  void operator()() { *ran = true; }
};

int main() {
  int n1{ 5 }, n2{ -2 };
  bool ran{ false };
  int sum{ 0 };
  std::thread t1{ f1, n1 };
  std::thread t2{ f2, std::ref( n2 ) };
  F3 f3{ &ran };
  std::thread t3{ f3 };
  std::thread t4{ [&sum] { for( int i = 1; i <= 5; ++i ) sum += i; } };
  t1.join(); t2.join(); t3.join(); t4.join();   // after join, the threads' writes are visible
  std::cout << "n1 = " << n1 << " (copy changed only), n2 = " << n2 << " (modified by reference), "
            << ( ran ? "F3 ran" : "F3 did not run" ) << ", lambda sum = " << sum << "\n";
}
