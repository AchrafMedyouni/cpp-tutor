#include <iomanip>
#include <iostream>
#include <vector>

class Forme {
public:
  virtual ~Forme() = default;
  virtual double aire() const { return 0; }
  virtual void dessine() const { std::cout << "Forme"; }
};

class Cercle : public Forme {
public:
  explicit Cercle( double r ) : r_{ r } {}
  double aire() const override { return 3.14159265358979 * r_ * r_; }
  void dessine() const override { std::cout << "Cercle(r=" << r_ << ")"; }
private:
  double r_;
};

class Rectangle : public Forme {
public:
  Rectangle( double l, double h ) : l_{ l }, h_{ h } {}
  double aire() const override { return l_ * h_; }
  void dessine() const override { std::cout << "Rectangle(" << l_ << "x" << h_ << ")"; }
private:
  double l_, h_;
};

int main() {
  std::vector<Forme*> formes{ new Cercle{ 2 }, new Rectangle{ 3, 4 }, new Cercle{ 1 } };
  double total = 0;
  for (const Forme* f : formes) {       // one loop, many behaviours
    f->dessine();
    std::cout << std::fixed << std::setprecision(2) << " area " << f->aire() << "\n"
              << std::defaultfloat << std::setprecision(6);   // fixed is sticky: reset it
    total += f->aire();
  }
  std::cout << std::fixed << std::setprecision(2) << "total area: " << total << "\n";
  for (Forme* f : formes) delete f;     // virtual destructor: each object fully destroyed
}
