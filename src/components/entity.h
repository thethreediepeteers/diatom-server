#pragma once

#include "controllers.h"
#include "modules/physics.h"
#include "modules/util.h"
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct Gun {
  std::string type;

  float offset;
  float offsetDirection;
  float reload;
  float bspeed;
  float angle;
  float length;
  float tick;

  int life;
};

class Entity {
public:
  static int counter;

  static std::map<int, Entity*> instances;
  static std::vector<int> toDelete;

  enum class ControlType { DefaultController, BulletController };

  Entity(double x, double y, float angle, uint8_t shape, util::HexColor c,
         hshg* grid, int clientId = -1);

  void spawn(const std::string& mockup, int t = counter,
             ControlType control = ControlType::DefaultController, int l = 0);
  void kill();
  void tick();
  void stayInBounds(int x, int y, int width, int height);
  void shoot();
  void define(std::string what);
  void addVel(const XY other);

  std::vector<uint8_t> encode() const;

  friend struct Controller;
  friend struct BulletController;
  friend class Client;

  std::unique_ptr<Controller> controller;

  int getId() const { return id; };
  XY getPos() const { return pos; };
  float getSize() const { return size; };

  bool getFlag(uint8_t index) const { return flags & (1 << index); }
  void setFlag(uint8_t index, bool f) {
    if (f) {
      flags |= (1 << index);
    } else {
      flags &= ~(1 << index);
    }
  }

private:
  int id;
  int life;
  int speed;
  int health;
  int damage;
  int maxHealth;
  int team;
  int mockupId;
  int clientId;

  bool autoFire;

  uint8_t flags; // 1: death, 2: remove from hshg

  float size;
  float angle;

  hshg* grid;
  uint8_t shape;

  std::vector<Gun> guns;
  util::HexColor color;

  XY pos;
  XY vel;
};
