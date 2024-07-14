#include "util.h"

std::string Util::trim(std::string& s) {
  const std::string whitespace = " \t\n\r\f\v";

  size_t firstNonSpace = s.find_first_not_of(whitespace);
  s.erase(0, firstNonSpace);

  size_t lastNonSpace = s.find_last_not_of(whitespace);
  s.erase(lastNonSpace + 1);

  return s;
}
