#include "controllers.h"
#include "mockups.h"
#include "modules/physics.h"
#include "modules/util.h"
#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct Gun {
  float offset, offsetDirection;
  int life;
  float reload;
  float bspeed;
  std::string bulletType;
  float angle;
  float length;

  float tick;
};

class Entity {
public:
  static std::map<int, Entity*> instances;
  static std::vector<int> toDelete;
  static int counter;

  Entity(double x, double y, float angle, uint8_t shape, util::HexColor c,
         hshg* grid, std::string controller = "default", int life = 0);
  virtual ~Entity();

  void tick();
  void stayInBounds(int x, int y, int width, int height);

  void shoot();
  void define(std::string what);

  std::vector<uint8_t> encode() const;

  int getId() const { return id; };
  XY getPos() const { return pos; };
  float getSize() const { return size; };

  void addVel(const XY other) { vel += other; };

  friend struct Controller;
  friend struct BulletController;

  Controller* controller;
  bool remove;

private:
  int id;

  hshg* grid;
  std::vector<Gun> guns;

protected:
  int mockupId;
  XY pos;
  float size;
  float angle;
  uint8_t shape;
  XY vel;
  util::HexColor color;

  int life;
};
