#include "modules/util.h"
#include <cstring>
#include <map>

struct Gun {
  short length;
  short width;
  float xOffset;
  float yOffset;
  float angle;
  std::vector<uint8_t> encode() const {
    std::vector<uint8_t> buffer(2 * sizeof(short) + 3 * sizeof(float));
    uint8_t* ptr = buffer.data();

    memcpy(ptr, &length, sizeof(short));
    ptr += sizeof(short);
    memcpy(ptr, &width, sizeof(short));
    ptr += sizeof(short);

    memcpy(ptr, &xOffset, sizeof(float));
    ptr += sizeof(float);
    memcpy(ptr, &yOffset, sizeof(float));
    ptr += sizeof(float);
    memcpy(ptr, &angle, sizeof(float));

    return buffer;
  }
};

struct Turret {
  short size;
  float xOffset;
  float yOffset;
  uint8_t shape;

  std::vector<uint8_t> encode() const {
    std::vector<uint8_t> buffer(sizeof(short) + 2 * sizeof(float) + 1);
    uint8_t* ptr = buffer.data();

    memcpy(ptr, &xOffset, sizeof(float));
    ptr += sizeof(float);
    memcpy(ptr, &yOffset, sizeof(float));
    ptr += sizeof(float);

    memcpy(ptr, &size, sizeof(short));
    ptr += sizeof(short);

    memcpy(ptr, &shape, 1);

    return buffer;
  }
};

struct Definition {
  static int counter;
  static std::map<std::string, Definition> definitions;

  int id;
  short size;
  uint8_t shape;
  std::vector<Gun> guns;
  std::vector<Turret> turrets;

  std::vector<uint8_t> encode() {
    int gunsSize = guns.size() * (2 * sizeof(short) + 3 * sizeof(float));
    int turretsSize = turrets.size() * (sizeof(short) + 2 * sizeof(float) + 1);

    std::vector<uint8_t> buffer(sizeof(int) + sizeof(short) + 1 + sizeof(int) +
                                gunsSize + sizeof(int) + turretsSize);
    uint8_t* ptr = buffer.data();

    memcpy(ptr, &id, sizeof(int));
    ptr += sizeof(int);

    memcpy(ptr, &size, sizeof(short));
    ptr += sizeof(short);
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

std::vector<uint8_t> generateMockups();
