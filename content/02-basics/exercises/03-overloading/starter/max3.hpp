#pragma once
#include <string>

// TODO: implement the four overloads
inline int max3(int a, int b) { (void)b; return a; }
inline int max3(int a, int b, int c) { (void)b; (void)c; return a; }
inline double max3(double a, double b, double c) { (void)b; (void)c; return a; }
inline std::string max3(const std::string& a, const std::string& b, const std::string& c) { (void)b; (void)c; return a; }
