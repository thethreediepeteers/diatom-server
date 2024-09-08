#include "controllers.h"
#include "mockups.h"
#include "modules/physics.h"
#include "modules/util.h"
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

struct Gun {
  float offset, offsetDirection;
  int life;
  float reload;
  float bspeed;
  float angle;
  float length;

  float tick;
};

class Entity {
public:
  static std::map<int, Entity*> instances;
  static std::vector<int> toDelete;
  static int counter;

  enum class ControlType { DefaultController, BulletController };

  Entity(double x, double y, float angle, uint8_t shape, util::HexColor c,
         hshg* grid);
  virtual ~Entity();

  void spawn(const std::string& mockup, int t = counter,
             ControlType control = ControlType::DefaultController, int l = 0);
  void kill();

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

  friend class Client;

  std::unique_ptr<Controller> controller;

  bool death, remove;

private:
  int id;

  hshg* grid;
  std::vector<Gun> guns;

  int mockupId;
  XY pos;
  float size;
  float angle;
  uint8_t shape;
  XY vel;
  util::HexColor color;

  int life;
  int speed;
  int health, maxHealth;

  int team;
};
