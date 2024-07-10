#include <map>

struct XY {
  int x, y;
  XY(int x, int y) : x(x), y(y) {
  }
};

class Entity {
public:
  static std::map<int, Entity *> instances;
  static int counter;

  Entity(int x, int y);
  ~Entity();

private:
  int id;
  XY pos;
  XY vel;
};
