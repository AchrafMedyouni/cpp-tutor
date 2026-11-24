#pragma once
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <string>

class Money {
public:
  Money(long cents, std::string currency) : cents_{ cents }, ccy_{ std::move(currency) } {}
  long cents() const { return cents_; }
  const std::string& currency() const { return ccy_; }
private:
  long cents_;
  std::string ccy_;
};

// TODO: operators
