int main() {
  int *a = new int{ 1 };
  int *b = a;          // two pointers, one object: who owns it?
  delete a;
  delete b;            // freed twice
}
