#include <iostream>

class Rectangle {
public:
  Rectangle(double l, double h) : l_{ l }, h_{ h } {}
  virtual ~Rectangle() = default;
  virtual void set_largeur(double l) { l_ = l; }
  virtual void set_longueur(double h) { h_ = h; }
  double largeur() const { return l_; }
  double surface() const { return l_ * h_; }
protected:
  double l_, h_;
};

class Carre : public Rectangle {
public:
  explicit Carre(double c) : Rectangle{ c, c } {}
  void set_largeur(double c) override { l_ = h_ = c; }   // must keep l_ == h_
  void set_longueur(double c) override { l_ = h_ = c; }
};

// Written for Rectangle: "doubling the width doubles the surface".
void double_width(Rectangle& r, const char* name) {
  double before = r.surface();
  r.set_largeur(2 * r.largeur());
  double after = r.surface();
  std::cout << name << ": area " << before << " -> " << after
            << " (expected x2: " << (after == 2 * before ? "ok" : "VIOLATED") << ")\n";
}

int main() {
  Rectangle r{ 2, 3 };
  Carre c{ 3 };
  double_width(r, "Rectangle");
  double_width(c, "Carre");      // substitutes fine for the compiler, not for the logic
}
