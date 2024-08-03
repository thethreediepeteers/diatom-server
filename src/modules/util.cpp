#include "util.h"

namespace util {
  HexColor HexColor::fromString(const std::string& hex) {
    uint8_t r = std::stoi(hex.substr(1, 2), 0, 16);
    uint8_t g = std ::stoi(hex.substr(3, 2), 0, 16);
    uint8_t b = std::stoi(hex.substr(5, 2), 0, 16);

    return HexColor(r, g, b);
  }

  std::vector<uint8_t> HexColor::encode() const {
    std::vector<uint8_t> buffer(3);
    buffer[0] = r;
    buffer[1] = g;
    buffer[2] = b;

    return buffer;
  }

  std::default_random_engine& getEngine() {
    static std::default_random_engine engine(std::random_device{}());
    return engine;
  }

  HexColor randcolor() {
    return HexColor(rand<uint8_t>(255), rand<uint8_t>(255), rand<uint8_t>(255));
  }
} // namespace util
