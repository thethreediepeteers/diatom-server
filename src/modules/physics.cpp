#include "physics.h"
#include <cstring>

std::vector<uint8_t> Map::encode() const {
  std::vector<uint8_t> buffer(sizeof(int) * 2);
  uint8_t* ptr = buffer.data();

  memcpy(ptr, &width, sizeof(width));
  ptr += sizeof(int);

  memcpy(ptr, &height, sizeof(height));

  return buffer;
}
