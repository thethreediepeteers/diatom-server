#include "entity.h"
#include "components/controllers.h"
#include <cstring>
#include <iostream>
#include <numbers>

// initalize static variables
std::map<int, Entity*> Entity::instances{};
std::vector<int> Entity::toDelete{};
int Entity::counter{};

Entity::Entity(double x, double y, float angle, uint8_t shape, util::HexColor c,
               hshg* grid, std::string control, int life)
    : remove(false), id(counter++), grid(grid), mockupId(0), pos(XY(x, y)),
      angle(angle), shape(shape), vel(XY(0, 0)), color(c), life(life) {
  instances[id] = this;

  define("base");
  hshg_insert(grid, pos.x, pos.y, size, id);

  if (control == "bullet") {
    controller = new BulletController(this);
  } else {
    controller = new Controller(this);
  }
}
Entity::~Entity() {
  delete controller;
  remove = true;
}

void Entity::tick() {
  controller->move();

  for (Gun& gun : guns) {
    ++gun.tick;
  }

  if (life != 0) {
    if (--life == 0) {
      toDelete.push_back(id);
    }
  }
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

void Entity::shoot() {
  for (Gun& gun : guns) {
    if (gun.tick >= gun.reload) {
      float gx = gun.offset * std::cos(gun.offsetDirection + gun.angle + angle);
      float gy = gun.offset * std::sin(gun.offsetDirection + gun.angle + angle);
      float gunEndX = gun.length * std::cos(gun.angle + angle);
      float gunEndY = gun.length * std::sin(gun.angle + angle);
      float bulletX = pos.x + gx + gunEndX;
      float bulletY = pos.y + gy + gunEndY;

      Entity* entity = new Entity(bulletX, bulletY, angle + gun.angle, 1, color,
                                  grid, "bullet", gun.life);

      entity->vel.x = std::cos(entity->angle) * 5 * gun.bspeed;
      entity->vel.y = std::sin(entity->angle) * 5 * gun.bspeed;

      gun.tick = 0;
    }
  }
}

void Entity::define(std::string what) {
  Definition def = Definition::definitions[what];

  mockupId = def.id;
  size = def.size;

  for (GunMockup gunm : def.guns) {
    Gun g;
    g.angle = gunm.angle * std::numbers::pi / 180;

    g.length = gunm.length;
    g.offset = gunm.offset;
    g.offsetDirection = gunm.direction;
    g.bspeed = gunm.body.bspeed;
    g.reload = gunm.body.reload;
    g.bulletType = gunm.body.reload;
    g.life = gunm.body.life;
    g.tick = g.reload;

    guns.push_back(g);
  }
}
