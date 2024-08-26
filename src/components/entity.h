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

  Entity(double x, double y, float angle, uint8_t shape, util::HexColor c,
         hshg* grid);
  virtual ~Entity();

  void tick();
  void stayInBounds(int x, int y, int width, int height);

  void define(std::string what);

  std::vector<uint8_t> encode() const;

  int getId() const { return id; };
  XY getPos() const { return pos; };
  float getSize() const { return size; };

  void addVel(const XY other) { vel += other; };

  bool remove;

private:
  int id;

  hshg* grid;

protected:
  int mockupId;
  XY pos;
  float size;
  float angle;
  uint8_t shape;

  util::HexColor color;

  XY vel;
};
