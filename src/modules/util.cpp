#include "util.h"
#include <random>

std::string util::trim(std::string& s) {
  const std::string whitespace = " \t\n\r\f\v";

  size_t firstNonSpace = s.find_first_not_of(whitespace);
  s.erase(0, firstNonSpace);

  size_t lastNonSpace = s.find_last_not_of(whitespace);
  s.erase(lastNonSpace + 1);

  return s;
}

int util::randint(int min, int max) {
  std::random_device rd;
  std::default_random_engine engine(rd());

  std::uniform_int_distribution<> dis(min, max);

  return dis(engine);
}

int util::randint(int max) { return randint(0, max); }
