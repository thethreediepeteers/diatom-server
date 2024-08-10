#include <cmath>
#include <cstdint>
#include <cstring>
#include <map>
#include <numbers>
#include <string>
#include <vector>

struct Gun {
  float length;
  float width;
  float xOffset;
  float yOffset;
  float angle;
  float aspect;

  std::vector<uint8_t> encode() const {
    std::vector<uint8_t> buffer(6 * sizeof(float));
    uint8_t* ptr = buffer.data();

    memcpy(ptr, &length, sizeof(float));
    ptr += sizeof(float);
    memcpy(ptr, &width, sizeof(float));
    ptr += sizeof(float);

    float offset = std::sqrt(std::pow(xOffset, 2) + std::pow(yOffset, 2));
    float direction = std::atan2(xOffset, yOffset);
    memcpy(ptr, &offset, sizeof(float));
    ptr += sizeof(float);
    memcpy(ptr, &direction, sizeof(float));
    ptr += sizeof(float);

    float a = angle * std::numbers::pi / 180;
    memcpy(ptr, &a, sizeof(float));
    ptr += sizeof(float);
    memcpy(ptr, &aspect, sizeof(float));

    return buffer;
  }
};

struct Turret {
  float size;
  float xOffset;
  float yOffset;
  float angle;
  uint8_t shape;

  std::vector<uint8_t> encode() const {
    std::vector<uint8_t> buffer(4 * sizeof(float) + 1);
    uint8_t* ptr = buffer.data();

    memcpy(ptr, &xOffset, sizeof(float));
    ptr += sizeof(float);
    memcpy(ptr, &yOffset, sizeof(float));
    ptr += sizeof(float);

    memcpy(ptr, &size, sizeof(float));
    ptr += sizeof(float);

    float a = angle * std::numbers::pi / 180;
    memcpy(ptr, &a, sizeof(float));
    ptr += sizeof(float);

    memcpy(ptr, &shape, 1);

    return buffer;
  }
};

struct Definition {
  static int counter;
  static std::map<std::string, Definition> definitions;

  int id;
  float size;
  uint8_t shape;
  std::vector<Gun> guns;
  std::vector<Turret> turrets;

  std::vector<uint8_t> encode() {
    int gunsSize = guns.size() * (6 * sizeof(float));
    int turretsSize = turrets.size() * (4 * sizeof(float) + 1);

    std::vector<uint8_t> buffer(sizeof(int) + sizeof(float) + 1 + sizeof(int) +
                                gunsSize + sizeof(int) + turretsSize);
    uint8_t* ptr = buffer.data();

    memcpy(ptr, &id, sizeof(int));
    ptr += sizeof(int);

    memcpy(ptr, &size, sizeof(float));
    ptr += sizeof(float);
    memcpy(ptr, &shape, 1);
    ptr += 1;

    int gs = guns.size();
    memcpy(ptr, &gs, sizeof(int));
    ptr += sizeof(int);

    for (auto& gun : guns) {
      std::vector<uint8_t> gunData = gun.encode();
      memcpy(ptr, gunData.data(), gunData.size());
      ptr += gunData.size();
    }

    int ts = turrets.size();
    memcpy(ptr, &ts, sizeof(int));
    ptr += sizeof(int);

    for (auto& turret : turrets) {
      std::vector<uint8_t> turretData = turret.encode();
      memcpy(ptr, turretData.data(), turretData.size());
      ptr += turretData.size();
    }

    return buffer;
  }
};

void generateMockups();
