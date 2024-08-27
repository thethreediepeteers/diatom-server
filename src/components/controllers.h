#pragma once

class Entity;

struct Controller {
  Controller(Entity* base) : base(base) {}
  virtual ~Controller() = default;

  virtual void move();
  virtual void collide(Entity* other);

  Entity* base;
};

struct BulletController : Controller {
  BulletController(Entity* base) : Controller(base) {}

  void move() override;
  void collide(Entity* other) override;
};
