// Linked first into every CppLab program.
// stdout is a pipe here, so the C library would fully buffer it: if the program then crashes
// (e.g. a sanitizer abort), everything it printed would be lost. Line buffering keeps each
// complete line, so the output shows how far the program got.
#include <cstdio>

namespace {
struct LineBufferedStdout {
  LineBufferedStdout() { std::setvbuf(stdout, nullptr, _IOLBF, BUFSIZ); }
} g_line_buffered_stdout;
}  // namespace
