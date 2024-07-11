#include "entity.h"

std::map<int, Entity *> Entity::instances{};
int Entity::counter{};

Entity::Entity(int x, int y) : id(counter++), pos(XY(x, y)), vel(XY(0, 0)) {
  instances[id] = this;
}

Entity::~Entity() { instances.erase(id); }
