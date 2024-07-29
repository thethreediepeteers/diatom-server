#include <cstdint>
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

  std::vector<uint8_t> serialize() {
    std::vector<uint8_t> buffer(2 * sizeof(double) + sizeof(float) +
                                sizeof(int));
    uint8_t* ptr = buffer.data();

    memcpy(ptr, &x, sizeof(double));
    ptr += sizeof(double);

    memcpy(ptr, &y, sizeof(double));
    ptr += sizeof(double);

    memcpy(ptr, &size, sizeof(float));
    ptr += sizeof(float);

    memcpy(ptr, &id, sizeof(int));

    return buffer;
  }
};

class Entity {
public:
  static std::map<int, Entity*> instances;
  static int counter;

  Entity(double x, double y, int size);
  virtual ~Entity();

  void tick();
  void stayInBounds(int x, int y, int width, int height);

  std::vector<uint8_t> encode();

private:
  int id;

protected:
  int getId() { return id; };

  XY pos;
  int size;
  XY vel;
};
