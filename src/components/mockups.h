#pragma once

#include <cmath>
#include <cstdint>
#include <cstring>
#include <map>
#include <string>
#include <vector>

struct GunBody {
  std::string type;
  float bspeed;
  float reload;

  int life;

  std::string bulletType;
};

struct Body {
  int damage;
  int health;
  int speed;
  bool autoFire;
};

struct GunMockup {
  float length;
  float width;
  float xOffset;
  float yOffset;
  float angle;
  float aspect;
  float offset;
  float direction;

  GunBody body;

  std::vector<uint8_t> encode();
};

struct TurretMockup {
  float size;
  float xOffset;
  float yOffset;
  float angle;
  uint8_t shape;

  std::vector<uint8_t> encode() const;
};

struct Definition {
  static int counter;
  static std::map<std::string, Definition> definitions;

  int id;
  float size;
  uint8_t shape;
  std::vector<GunMockup> guns;
  std::vector<TurretMockup> turrets;

  uint8_t layer;
  Body body;

  std::vector<uint8_t> encode();
};

void generateMockups();
