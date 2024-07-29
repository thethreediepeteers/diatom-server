#include "entity.h"
#include <iostream>
#include <vector>

std::map<int, Entity*> Entity::instances{};
int Entity::counter{};

Entity::Entity(double x, double y, int s)
    : id(counter++), pos(XY(x, y)), size(s), vel(XY(0, 0)) {
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
std::vector<uint8_t> Entity::encode() {
  return EntityState(id, pos.x, pos.y, size).serialize();
}
