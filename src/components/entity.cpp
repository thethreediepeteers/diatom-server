#include "entity.h"
#include "components/controllers.h"
#include "mockups.h"
#include "modules/config.h"
#include <csignal>
#include <cstring>

// initalize static variables
std::map<int, Entity*> Entity::instances{};
std::vector<int> Entity::toDelete{};

int Entity::counter{};

Entity::Entity(double x, double y, float angle, uint8_t shape, util::HexColor c,
               hshg* grid)
    : death(true), remove(false), id(counter++), health(0), maxHealth(0),
      mockupId(-1), angle(angle), grid(grid), shape(shape), color(c),
      pos(XY(x, y)), vel(XY(0, 0)) {
  instances[id] = this;
}

void Entity::spawn(const std::string& mockup, int t, ControlType control,
                   int l) {
  death = false;

  define(mockup);
  hshg_insert(grid, pos.x, pos.y, size, id);

  team = t;
  life = l;

  switch (control) {
  case ControlType::BulletController:
    controller = std::make_unique<BulletController>(this);
    break;

  default:
    controller = std::make_unique<Controller>(this);
    break;
  }
}

void Entity::kill() {
  toDelete.push_back(id);

  remove = true;

  if (!death) {
    death = true;
  }
}

void Entity::tick() {
  controller->move();

  for (Gun& gun : guns) {
    ++gun.tick;
  }

  if ((life > 0 && --life == 0) || health <= 0) {
    kill();
  }
}

void Entity::stayInBounds(int x, int y, int width, int height) {
  if (pos.x < 0) {
    vel.x -= (pos.x) / config::ROOM_BOUNCE;
  } else if (pos.x > width) {
    vel.x -= (pos.x - width) / config::ROOM_BOUNCE;
  }

  if (pos.y < 0) {
    vel.y -= (pos.y) / config::ROOM_BOUNCE;
  } else if (pos.y > height) {
    vel.y -= (pos.y - height) / config::ROOM_BOUNCE;
  }
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

      Entity* entity =
          new Entity(bulletX, bulletY, angle + gun.angle, 1, color, grid);

      entity->spawn("bullet", team, ControlType::BulletController, gun.life);

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
  speed = def.body.speed;
  health = maxHealth = def.body.health;

  guns.clear();

  for (GunMockup gunm : def.guns) {
    Gun g;
    g.angle = gunm.angle * std::numbers::pi / 180;
    g.length = gunm.length;
    g.offset = gunm.offset;
    g.offsetDirection = gunm.direction;
    g.bspeed = gunm.body.bspeed;
    g.reload = gunm.body.reload;
    g.life = gunm.body.life;
    g.tick = g.reload;

    guns.push_back(g);
  }

  hshg_insert(grid, pos.x, pos.y, size, id);
}

void Entity::addVel(const XY other) { vel += other; };

std::vector<uint8_t> Entity::encode() const {
  std::vector<uint8_t> buffer(2 * sizeof(double) + 2 * sizeof(float) +
                              sizeof(int) * 5 + 4 /*shape (1) + color (3)*/);

  uint8_t* ptr = buffer.data();

  std::memcpy(ptr, &pos.x, sizeof(double));
  ptr += sizeof(double);
  std::memcpy(ptr, &pos.y, sizeof(double));
  ptr += sizeof(double);
  std::memcpy(ptr, &size, sizeof(float));
  ptr += sizeof(float);
  std::memcpy(ptr, &angle, sizeof(float));
  ptr += sizeof(float);
  std::memcpy(ptr, &id, sizeof(int));
  ptr += sizeof(int);
  std::memcpy(ptr, &mockupId, sizeof(int));
  ptr += sizeof(int);
  std::memcpy(ptr, &health, sizeof(int));
  ptr += sizeof(int);
  std::memcpy(ptr, &maxHealth, sizeof(int));
  ptr += sizeof(int);
  std::memcpy(ptr, &team, sizeof(int));
  ptr += sizeof(int);
  std::memcpy(ptr, &shape, 1);
  ptr += 1;
  std::memcpy(ptr, color.encode().data(), 3);

  return buffer;
}
