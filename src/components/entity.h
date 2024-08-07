#include "mockups.h"
#include "modules/physics.h"
#include "modules/util.h"
#include <cstdint>
#include <map>
#include <vector>

class Entity {
public:
  static std::map<int, Entity*> instances;
  static int counter;

  Entity(double x, double y, float angle, uint8_t shape, util::HexColor c);
  virtual ~Entity();

  void tick();
  void stayInBounds(int x, int y, int width, int height);

  void define(std::string what);

  std::vector<uint8_t> encode() const;

private:
  int id;

protected:
  int getId() const { return id; };
  int mockupId;
  XY pos;
  float size;
  float angle;
  uint8_t shape;

  util::HexColor color;

  XY vel;
};
