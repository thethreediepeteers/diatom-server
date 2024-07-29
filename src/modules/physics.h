#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

struct Map {
  Map(int w, int h) : width(w), height(h) {}
  int width, height;

  std::vector<uint8_t> encode() {
    std::vector<uint8_t> buffer(sizeof(int) * 2);
    uint8_t* ptr = buffer.data();

    memcpy(ptr, &width, sizeof(width));
    ptr += sizeof(int);

    memcpy(ptr, &height, sizeof(height));

    return buffer;
  }
};
