#include <iostream>

int global_count;                 // static storage, zero-initialised

int counter() {
  static int calls{ 0 };          // static storage too, initialised once
  return ++calls;
}

int main() {
  int local{ 42 };                // automatic: on the stack
  int *heap{ new int{ 7 } };      // dynamic: on the heap

  std::cout << "code   (counter):     " << reinterpret_cast<void*>(&counter) << "\n";
  std::cout << "global (global_count): " << &global_count << " value " << global_count << "\n";
  std::cout << "heap   (*heap):        " << heap << "\n";
  std::cout << "stack  (local):        " << &local << "\n";
  std::cout << "counter: " << counter() << counter() << counter() << "\n";
  delete heap;
}
