#include "controllers.h"
#include "entity.h"
#include "modules/config.h"
#include <cmath>

void Controller::move() {
  base->pos += base->vel;
  base->vel *= 0.8;

  base->stayInBounds(0, 0, config::MAP_WIDTH, config::MAP_HEIGHT);
}
void Controller::collide(Entity* other) {
  float dx = base->pos.x - other->pos.x;
  float dy = base->pos.y - other->pos.y;
  float distance = dx * dx + dy * dy;

  if (distance <= (base->size + other->size) * (base->size + other->size)) {
    float angle = atan2(dy, dx);
    float cosa = cos(angle);
    float sina = sin(angle);

    base->vel += {cosa, sina};
    other->vel += {-cosa, -sina};
  }
}

void BulletController::move() {
  base->pos += base->vel;

  base->stayInBounds(0, 0, config::MAP_WIDTH, config::MAP_HEIGHT);
}
void BulletController::collide(Entity* other) {}
