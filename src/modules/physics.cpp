#include "physics.h"
#include "components/entity.h"
#include "config.h"
#include <cmath>
#include <cstring>

Map::Map(int width, int height) : width(width), height(height) {}

std::vector<uint8_t> Map::encode() const {
  std::vector<uint8_t> buffer(sizeof(int) * 2);

  uint8_t* ptr = buffer.data();

  std::memcpy(ptr, &width, sizeof(width));
  ptr += sizeof(int);
  std::memcpy(ptr, &height, sizeof(height));

  return buffer;
}

hshg* initHSHG() {
  hshg* grid = hshg_create(config::MAP_WIDTH / 16, 16);
  grid->update = updateHSHG;
  grid->collide = collideHSHG;

  return grid;
}

void updateHSHG(hshg* h, hshg_entity* entity) {
  if (!Entity::instances.count(entity->ref)) {
    hshg_remove(h);
    return;
  }
  Entity* e = Entity::instances.at(entity->ref);

  if (e->getFlag(1)) {
    hshg_remove(h);
    e->setFlag(1, false);

    return;
  }

  entity->x = e->getPos().x;
  entity->y = e->getPos().y;

  hshg_move(h);
}

void collideHSHG(const hshg* /*hshg*/, const hshg_entity* a,
                 const hshg_entity* b) {
  if (!Entity::instances.count(a->ref) || !Entity::instances.count(b->ref)) {
    return;
  }
  Entity* ea = Entity::instances.at(a->ref);
  Entity* eb = Entity::instances.at(b->ref);

  ea->controller->collide(eb);
  eb->controller->collide(ea);
};
