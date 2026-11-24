#pragma once

struct Complexe {
  double real;
  double imag;

  double module() const;              // defined in Complexe.cpp
  double module_explicit_this() const;

  void ajoute( Complexe other ) {     // defined in the class: implicitly inline
    real += other.real;
    imag += other.imag;
  }
};
