#include "controllers.h"
#include "entity.h"
#include "modules/config.h"
#include <cmath>
#include <iostream>
#include <numbers>

const float weirdfactor = 2;

Controller::Controller(Entity* base) : base(base) {}

void Controller::move() {
  base->pos += base->vel * weirdfactor * base->speed / 2;
  base->vel *= 0.8;

  base->stayInBounds(0, 0, config::MAP_WIDTH, config::MAP_HEIGHT);
}

void Controller::collide(Entity* other) {
  // TODO: better checks for whether to do collision or damage

  if (base->team == other->team) {
    return;
  }

  float dx = base->pos.x - other->pos.x;
  float dy = base->pos.y - other->pos.y;
  float distance = dx * dx + dy * dy;

  if (distance <= (base->size + other->size) * (base->size + other->size)) {
    float angle = atan2(dy, dx);
    float cosa = cos(angle);
    float sina = sin(angle);

    base->vel += {cosa, sina};
    other->vel += {-cosa, -sina};

    base->health -= other->damage;
    other->health -= base->damage;
  }
}

BulletController::BulletController(Entity* base) : Controller(base) {}

void BulletController::move() {
  base->pos += base->vel * weirdfactor;

  // base->stayInBounds(0, 0, config::MAP_WIDTH, config::MAP_HEIGHT);
}

void BulletController::collide(Entity* other) {
  // TODO: better checks for whether to do collision or damage

  if (base->team == other->team) {
    return;
  }

  float dx = base->pos.x - other->pos.x;
  float dy = base->pos.y - other->pos.y;
  float distance = dx * dx + dy * dy;

  if (distance <= (base->size + other->size) * (base->size + other->size)) {
    base->health -= other->damage;
    other->health -= base->damage;
  }
}
