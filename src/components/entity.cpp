#include "entity.h"
#include <cstring>

// initalize static variables
std::map<int, Entity*> Entity::instances{};
int Entity::counter{};

Entity::Entity(double x, double y, float angle, uint8_t shape, util::HexColor c,
               hshg* grid)
    : remove(false), id(counter++), grid(grid), mockupId(0), pos(XY(x, y)),
      angle(angle), shape(shape), color(c), vel(XY(0, 0)) {
  instances[id] = this;

  define("base");
  hshg_insert(grid, pos.x, pos.y, size, id);
}
Entity::~Entity() { remove = true; }

void Entity::tick() {
  pos += vel;
  vel *= 0.8;

  stayInBounds(0, 0, config::MAP_WIDTH, config::MAP_HEIGHT);
}

void Entity::stayInBounds(int x, int y, int width, int height) {
  if (pos.x < 0)
    vel.x -= (pos.x) / config::ROOM_BOUNCE;
  else if (pos.x > width)
    vel.x -= (pos.x - width) / config::ROOM_BOUNCE;

  if (pos.y < 0)
    vel.y -= (pos.y) / config::ROOM_BOUNCE;
  else if (pos.y > height)
    vel.y -= (pos.y - height) / config::ROOM_BOUNCE;
}

std::vector<uint8_t> Entity::encode() const {
  std::vector<uint8_t> buffer(2 * sizeof(double) + 2 * sizeof(float) +
                              sizeof(int) * 2 + 4 /*shape (1) + color (3)*/);
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

  memcpy(ptr, &mockupId, sizeof(int));
  ptr += sizeof(int);

  memcpy(ptr, &shape, 1);
  ptr += 1;

  memcpy(ptr, color.encode().data(), 3);

  return buffer;
}

void Entity::define(std::string what) {
  Definition def = Definition::definitions[what];

  mockupId = def.id;
  size = def.size;
}
