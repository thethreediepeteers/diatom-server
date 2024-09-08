#include "physics.h"
#include "components/entity.h"
#include "config.h"
#include <cmath>
#include <cstring>

std::vector<uint8_t> Map::encode() const {
  std::vector<uint8_t> buffer(sizeof(int) * 2);
  uint8_t* ptr = buffer.data();

  memcpy(ptr, &width, sizeof(width));
  ptr += sizeof(int);

  memcpy(ptr, &height, sizeof(height));

  return buffer;
}

hshg* initHSHG() {
  hshg* grid = hshg_create(config::MAP_WIDTH, 16);
  grid->update = updateHSHG;
  grid->collide = collideHSHG;

  return grid;
}
void updateHSHG(hshg* h, hshg_entity* entity) {
  auto it = Entity::instances.find(entity->ref);
  if (it == Entity::instances.end()) {
    hshg_remove(h);
    return;
  }

  Entity* e = it->second;

  if (e->remove) {
    hshg_remove(h);
    e->remove = false;
    return;
  }

  entity->x = e->getPos().x;
  entity->y = e->getPos().y;

  hshg_move(h);
}
void collideHSHG(const hshg* hshg, const hshg_entity* a, const hshg_entity* b) {
  Entity* ea = Entity::instances[a->ref];
  Entity* eb = Entity::instances[b->ref];

  ea->controller->collide(eb);
  eb->controller->collide(ea);
};
