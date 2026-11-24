#include <iostream>
#include <thread>

int main() {
  std::cout << "hardware threads: " << std::thread::hardware_concurrency() << "\n";
  std::cout << "main thread id: " << std::this_thread::get_id() << "\n";
}
