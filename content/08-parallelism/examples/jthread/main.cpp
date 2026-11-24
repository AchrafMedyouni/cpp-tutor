#include <iostream>
#include <thread>
#include <vector>

int main() {
  std::vector<long> partial( 2, 0 );
  {
    std::jthread a{ [&] { for( long i = 1; i <= 100; ++i ) partial[0] += i; } };     // each thread writes its own slot
    std::jthread b{ [&] { for( long i = 101; i <= 200; ++i ) partial[1] += i; } };
  }   // both joined here by their destructors
  std::cout << "partial sums: " << partial[0] << " " << partial[1] << "\n";
  std::cout << "total: " << partial[0] + partial[1] << "\n";
}
