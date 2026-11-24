#pragma once
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "instruments.hpp"

inline std::unique_ptr<Instrument> make_instrument(const std::string& spec) { (void)spec; return nullptr; }   // TODO
inline double book_value(const std::vector<std::unique_ptr<Instrument>>& book) { (void)book; return 0; }        // TODO
inline std::unique_ptr<Instrument> take_largest(std::vector<std::unique_ptr<Instrument>>& book) { (void)book; return nullptr; }   // TODO
