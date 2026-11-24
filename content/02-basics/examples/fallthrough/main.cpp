#include <iostream>

enum MsgType { BUY = 'B', CANCEL = 'C', AMEND = 'A' };

void handle(char type) {
  switch (type) {
    case BUY:
      std::cout << "BUY -> buy side\n";
      break;
    case CANCEL:
    case AMEND:               // intentional: AMEND and CANCEL share code
      std::cout << (type == CANCEL ? "CANCEL" : "AMEND") << " -> cancel request\n";
      // BUG: missing break, falls into default
    default:
      std::cout << "unknown!\n";
  }
}

int main() {
  handle('B');
  handle('C');
  handle('A');
}
