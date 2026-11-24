#include <thread>

void reset( double & pnl ) { pnl = 0; }

int main() {
  double pnl{ 1234.5 };
  std::thread t{ reset, pnl };   // the thread stores a COPY: can't bind it to double&
  t.join();
}
