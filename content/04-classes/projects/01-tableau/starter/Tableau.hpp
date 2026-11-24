#pragma once
#include <stdexcept>
#include <utility>

class Tableau {
public:
  explicit Tableau(unsigned taille) { (void)taille; }   // TODO (step 1)
  double& get(unsigned indice) { (void)indice; return dummy_; }   // TODO (step 1)
  unsigned taille() const { return 0; }                 // TODO (step 1)
  // TODO (step 2): destructor
  // TODO (step 3): copy constructor
  // TODO (step 4): copy assignment (copy-and-swap)
private:
  double dummy_ = 0;
};
