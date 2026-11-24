#include <iostream>
#include <limits>

template <typename T>
void row(const char* name) {
  std::cout << name << ": " << sizeof(T) << " bytes, range ["
            << +std::numeric_limits<T>::lowest() << ", " << +std::numeric_limits<T>::max() << "]\n";
}

int main() {
  row<char>("char");
  row<short>("short");
  row<int>("int");
  row<long>("long");
  row<long long>("long long");
  row<unsigned>("unsigned");
  row<float>("float");
  row<double>("double");
  std::cout << "bool: " << sizeof(bool) << " byte\n";
  std::cout << "0b10010011 = " << 0b10010011 << ", 0xFE23U = " << 0xFE23U << ", 015 = " << 015 << " (octal!)\n";
}
