#pragma once

class Entity;

struct Controller {
  Controller(Entity* base);

  virtual void move();
  virtual void collide(Entity* other);

  Entity* base;
};

struct BulletController : Controller {
  BulletController(Entity* base);

  void move() override;
  void collide(Entity* other) override;
};
