#include "entity.h"
#include <nlohmann/json.hpp>

std::map<int, Entity*> Entity::instances{};
int Entity::counter{};

Entity::Entity(int x, int y) : id(counter++), pos(XY(x, y)), vel(XY(0, 0)) {
  instances[id] = this;
}
Entity::~Entity() { instances.erase(id); }

void Entity::tick() {
  pos += vel;
  vel *= 0.8;
}

nlohmann::json Entity::encode() {
  return {{"id", id}, {"pos", {{"x", pos.x}, {"y", pos.y}}}};
}
