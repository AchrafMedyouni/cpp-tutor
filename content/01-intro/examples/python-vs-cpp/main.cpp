#include <iostream>

/* A block comment can
   span several lines. */
int main() {
  int total = 0;               // the type is fixed: total is an int forever
  for (int i = 1; i <= 4; ++i) // braces, not indentation, define the block
  {
    total += i;
  }
  int a = 1; int b = 2;        // two statements on one line: legal (not pretty)
  std::cout << "total = " << total
            << ", a + b = "    // one statement spread over three lines
            << a + b << "\n";
  return 0;                    // exit code of the process
}
