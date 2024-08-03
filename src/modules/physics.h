#include <cstdint>
#include <vector>

struct XY {
  double x, y;
  XY(double x, double y) : x(x), y(y) {}

  void operator+=(const XY& other) {
    x += other.x;
    y += other.y;
  }
  void operator*=(double n) {
    x *= n;
    y *= n;
  }
};

struct Map {
  int width, height;

  Map(int width, int height) : width(width), height(height) {}

  std::vector<uint8_t> encode() const;
};
