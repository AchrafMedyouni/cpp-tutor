#include <cstring>
#include <iostream>
#include <string>

int main() {
  char s[] = "Hello world!";   // 12 characters + '\0'
  char t[] = "Hello world!";
  std::cout << "length " << std::strlen(s) << ", sizeof " << sizeof(s) << "\n";
  if (std::strcmp(s, t) == 0) std::cout << "strcmp == 0 → equal contents\n";

  char buf[32];
  std::strcpy(buf, s);         // no = for arrays: copy character by character
  std::cout << "copied: " << buf << "\n";

  std::string modern{ s };     // convert as early as possible
  std::cout << "as std::string: " << modern << "\n";
}
