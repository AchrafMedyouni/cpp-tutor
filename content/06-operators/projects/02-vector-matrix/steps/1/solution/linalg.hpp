#pragma once
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <vector>

class Vector {
public:
  explicit Vector(std::size_t n = 0) : v_(n, 0.0) {}
  Vector(std::initializer_list<double> l) : v_(l) {}
  std::size_t size() const { return v_.size(); }
  double& operator[](std::size_t i) { return v_.at(i); }
  const double& operator[](std::size_t i) const { return v_.at(i); }
  Vector& operator+=(const Vector& o) { check(o); for (std::size_t i = 0; i < size(); ++i) v_[i] += o.v_[i]; return *this; }
  Vector& operator-=(const Vector& o) { check(o); for (std::size_t i = 0; i < size(); ++i) v_[i] -= o.v_[i]; return *this; }
  Vector& operator*=(double k) { for (double& x : v_) x *= k; return *this; }
  bool operator==(const Vector&) const = default;
private:
  void check(const Vector& o) const { if (o.size() != size()) throw std::invalid_argument{ "size mismatch" }; }
  std::vector<double> v_;   // rule of zero: std::vector manages the memory
};

inline Vector operator+(Vector a, const Vector& b) { return a += b; }
inline Vector operator-(Vector a, const Vector& b) { return a -= b; }
inline Vector operator*(Vector a, double k) { return a *= k; }
inline Vector operator*(double k, Vector a) { return a *= k; }

inline double dot(const Vector& a, const Vector& b) {
  if (a.size() != b.size()) throw std::invalid_argument{ "size mismatch" };
  double s = 0;
  for (std::size_t i = 0; i < a.size(); ++i) s += a[i] * b[i];
  return s;
}
