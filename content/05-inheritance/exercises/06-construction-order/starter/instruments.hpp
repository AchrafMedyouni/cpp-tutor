#pragma once
#include <string>
#include <vector>

inline std::vector<std::string>& trace() { static std::vector<std::string> t; return t; }

class Instrument {
public:
  explicit Instrument(std::string name) : name_{ std::move(name) } { trace().push_back("Instrument(" + name_ + ")"); }
  virtual ~Instrument() { trace().push_back("~Instrument"); }
  const std::string& name() const { return name_; }
private:
  std::string name_;
};

class Bond : public Instrument {
public:
  // TODO: Bond(std::string name, double coupon), ~Bond(), coupon()
};
