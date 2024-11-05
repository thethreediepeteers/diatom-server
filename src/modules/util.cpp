#include "util.h"
#include <cassert>
#include <cstdint>

namespace util {
  HexColor::HexColor() {
    *this =
        HexColor(rand<uint8_t>(255), rand<uint8_t>(255), rand<uint8_t>(255));
  }

  HexColor::HexColor(const std::string& hex) {
    assert(hex.size() == 7 && hex[0] == '#');

    r = std::stoi(hex.substr(1, 2), 0, 16);
    g = std::stoi(hex.substr(3, 2), 0, 16);
    b = std::stoi(hex.substr(5, 2), 0, 16);
  }

  std::vector<uint8_t> HexColor::encode() const { return {r, g, b}; }

  std::default_random_engine& getEngine() {
    static std::default_random_engine engine(std::random_device{}());

    return engine;
  }

  void writeStringToBuffer(std::vector<uint8_t>& buffer,
                           const std::string& string) {
    uint8_t length = string.size();
    buffer.push_back(length);
    buffer.insert(buffer.end(), string.begin(), string.end());
  }
} // namespace util
