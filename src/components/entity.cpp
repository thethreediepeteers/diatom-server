#include "entity.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <rapidjson/document.h>

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
rapidjson::Document Entity::encode() {
  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartObject();
  writer.Key("id");
  writer.Int(id);
  writer.Key("pos");
  writer.StartObject();
  writer.Key("x");
  writer.Double(pos.x);
  writer.Key("y");
  writer.Double(pos.y);
  writer.EndObject();
  writer.Key("size");
  writer.Int(size);
  writer.EndObject();

  rapidjson::Document d;
  d.Parse(s.GetString());

  return d;
}
