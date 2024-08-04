#include "entity.h"
#include <cstring>

// initalize static variables
std::map<int, Entity*> Entity::instances{};
int Entity::counter{};

Entity::Entity(double x, double y, float s, uint8_t shape, util::HexColor c)
    : id(counter++), pos(XY(x, y)), size(s), shape(shape), angle(0), color(c),
      vel(XY(0, 0)) {
  instances[id] = this;
}
Entity::~Entity() {}

void Entity::tick() {
  pos += vel;
  vel *= 0.8;
}

void Entity::stayInBounds(int x, int y, int width, int height) {
  if (pos.x - size < 0)
    vel.x += (pos.x) / 3;
  else if (pos.x + size > width)
    vel.x += (pos.x - width) / 3;

  if (pos.y - size < 0)
    vel.y += (pos.y) / 3;
  else if (pos.y + size > height)
    vel.y += (pos.y - height) / 3;
}
std::vector<uint8_t> Entity::encode() const {
  std::vector<uint8_t> buffer(2 * sizeof(double) + 2 * sizeof(float) +
                              sizeof(int) + 4 /*shape (1) + color (3)*/);
  uint8_t* ptr = buffer.data();

  memcpy(ptr, &pos.x, sizeof(double));
  ptr += sizeof(double);

  memcpy(ptr, &pos.y, sizeof(double));
  ptr += sizeof(double);

  memcpy(ptr, &size, sizeof(float));
  ptr += sizeof(float);

  memcpy(ptr, &angle, sizeof(float));
  ptr += sizeof(float);

  memcpy(ptr, &id, sizeof(int));
  ptr += sizeof(int);

  memcpy(ptr, &shape, 1);
  ptr += 1;

  memcpy(ptr, color.encode().data(), 3);

  return buffer;
}
