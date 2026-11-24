struct Complexe {
  double real;
  double imag;
};

int main() {
  Complexe a{ 1, 2 }, b{ 1, 2 };
  return a == b ? 0 : 1;   // error: no operator== for Complexe
}
