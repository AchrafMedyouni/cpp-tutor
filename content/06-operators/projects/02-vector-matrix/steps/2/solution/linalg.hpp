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

class Matrix {
public:
  Matrix(std::size_t rows, std::size_t cols) : r_{ rows }, c_{ cols }, a_(rows * cols, 0.0) {}
  Matrix(std::initializer_list<std::initializer_list<double>> rows) : r_{ rows.size() }, c_{ rows.size() ? rows.begin()->size() : 0 } {
    for (const auto& row : rows) {
      if (row.size() != c_) throw std::invalid_argument{ "ragged matrix" };
      a_.insert(a_.end(), row.begin(), row.end());
    }
  }
  std::size_t rows() const { return r_; }
  std::size_t cols() const { return c_; }
  double& operator()(std::size_t i, std::size_t j) { check(i, j); return a_[i * c_ + j]; }
  const double& operator()(std::size_t i, std::size_t j) const { check(i, j); return a_[i * c_ + j]; }
  bool operator==(const Matrix&) const = default;

  Matrix transpose() const {
    Matrix t{ c_, r_ };
    for (std::size_t i = 0; i < r_; ++i)
      for (std::size_t j = 0; j < c_; ++j) t(j, i) = (*this)(i, j);
    return t;
  }

private:
  void check(std::size_t i, std::size_t j) const { if (i >= r_ || j >= c_) throw std::out_of_range{ "Matrix index" }; }
  std::size_t r_, c_;
  std::vector<double> a_;   // row-major
};

inline Vector operator*(const Matrix& m, const Vector& v) {
  if (m.cols() != v.size()) throw std::invalid_argument{ "dimension mismatch" };
  Vector out(m.rows());
  for (std::size_t i = 0; i < m.rows(); ++i)
    for (std::size_t j = 0; j < m.cols(); ++j) out[i] += m(i, j) * v[j];
  return out;
}

inline Matrix operator*(const Matrix& a, const Matrix& b) {
  if (a.cols() != b.rows()) throw std::invalid_argument{ "dimension mismatch" };
  Matrix out{ a.rows(), b.cols() };
  for (std::size_t i = 0; i < a.rows(); ++i)
    for (std::size_t k = 0; k < a.cols(); ++k)
      for (std::size_t j = 0; j < b.cols(); ++j) out(i, j) += a(i, k) * b(k, j);
  return out;
}
