#pragma once

#include <cstdint>
#include <random>
#include <string>
#include <vector>

namespace util {
  struct HexColor {
    uint8_t r, g, b;

    HexColor();
    HexColor(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {}
    HexColor(const std::string& hex);

    std::vector<uint8_t> encode() const;
  };

  std::default_random_engine& getEngine();

  template <typename T>
  T rand(T min, T max) {
    std::default_random_engine& engine = getEngine();

    if constexpr (std::is_integral<T>::value) {
      std::uniform_int_distribution<T> dis(min, max);

      return dis(engine);
    } else if constexpr (std::is_floating_point<T>::value) {
      std::uniform_real_distribution<T> dis(min, max);

      return dis(engine);
    }
  }

  template <typename T>
  T rand(T max) {
    return rand<T>(0, max);
  }

  HexColor randcolor();
} // namespace util
