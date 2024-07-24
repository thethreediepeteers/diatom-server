#include <map>
#include <nlohmann/json.hpp>

using nlohmann::json;

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

class Entity {
public:
  static std::map<int, Entity*> instances;
  static int counter;

  Entity(int x, int y);
  ~Entity();

  void tick();

  json encode();

private:
  int id;

protected:
  XY pos;
  XY vel;
};
