#include <cstring>
#include <iostream>
#include <map>
#include <rapidjson/document.h>
#include <string>
#include <vector>

struct XY {
  double x, y;
  XY(double x, double y) : x(x), y(y) {}

  void operator+=(const XY& other) {
    x += other.x;
    y += other.y;
  }
  void operator*=(double n) {
    x *= n;
    y *= n;
  }
};

struct EntityState {
  EntityState(int id, double x, double y, float size)
      : id(id), x(x), y(y), size(size) {}
  int id;
  double x, y;
  float size;

  unsigned char* serialize() { return reinterpret_cast<unsigned char*>(this); }
};

class Entity {
public:
  static std::map<int, Entity*> instances;
  static int counter;

  Entity(double x, double y, int size);
  virtual ~Entity();

  void tick();
  void stayInBounds(int x, int y, int width, int height);

  rapidjson::Document encode();

private:
  int id;

protected:
  int getId() { return id; };

  XY pos;
  int size;
  XY vel;
};
