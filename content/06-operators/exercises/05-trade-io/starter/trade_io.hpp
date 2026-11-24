#pragma once
#include <iomanip>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

struct Trade { std::string symbol; bool buy; long qty; double price; };

// TODO: operator<< and operator>>
