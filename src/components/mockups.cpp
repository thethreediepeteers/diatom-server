#include "mockups.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

Definition bullet = {
    .size = 20, .shape = 0, .layer = 1, .body = {.damage = 1, .health = 10}};

Definition bigBullet = {
    .size = 20,
    .shape = 0,
    .guns =
        {
            {.length = 20,
             .width = 10,
             .aspect = 1,
             .body = {.type = "bullet", .bspeed = 5, .reload = 20, .life = 25}},
        },
    .turrets = {{.size = 14, .angle = 45}},
    .layer = 1,
    .body = {.damage = 1, .health = 10, .autoFire = true}};

Definition aggressor = {.size = 32.5,
                        .shape = 0,
                        .guns = {{.length = 30,
                                  .width = 17.5,
                                  .aspect = 1,
                                  .body = {.type = "bigBullet",
                                           .bspeed = 1,
                                           .reload = 10,
                                           .life = 50}}},
                        .layer = 0,
                        .body = {.damage = 1, .health = 50, .speed = 2}};

std::map<std::string, Definition> Definition::definitions = {
    {"bullet", bullet}, {"aggressor", aggressor}, {"bigBullet", bigBullet}};

std::vector<uint8_t> GunMockup::encode() {
  std::vector<uint8_t> buffer(6 * sizeof(float));

  offset = std::sqrt(std::pow(xOffset, 2) + std::pow(yOffset, 2));
  direction = std::atan2(xOffset, yOffset);

  float a = angle * std::numbers::pi / 180;
  uint8_t* ptr = buffer.data();

  std::memcpy(ptr, &length, sizeof(float));
  ptr += sizeof(float);
  std::memcpy(ptr, &width, sizeof(float));
  ptr += sizeof(float);
  std::memcpy(ptr, &offset, sizeof(float));
  ptr += sizeof(float);
  std::memcpy(ptr, &direction, sizeof(float));
  ptr += sizeof(float);
  std::memcpy(ptr, &a, sizeof(float));
  ptr += sizeof(float);
  std::memcpy(ptr, &aspect, sizeof(float));

  return buffer;
}

std::vector<uint8_t> TurretMockup::encode() const {
  std::vector<uint8_t> buffer(4 * sizeof(float) + 1);

  float a = angle * std::numbers::pi / 180;
  uint8_t* ptr = buffer.data();

  std::memcpy(ptr, &xOffset, sizeof(float));
  ptr += sizeof(float);
  std::memcpy(ptr, &yOffset, sizeof(float));
  ptr += sizeof(float);
  std::memcpy(ptr, &size, sizeof(float));
  ptr += sizeof(float);
  std::memcpy(ptr, &a, sizeof(float));
  ptr += sizeof(float);
  std::memcpy(ptr, &shape, 1);

  return buffer;
}

int Definition::counter = 0;

std::vector<uint8_t> Definition::encode() {
  int gunsSize = guns.size() * (6 * sizeof(float));
  int turretsSize = turrets.size() * (4 * sizeof(float) + 1);
  int gs = guns.size();
  int ts = turrets.size();

  std::vector<uint8_t> buffer(sizeof(int) + sizeof(float) + 2 + sizeof(int) +
                              gunsSize + sizeof(int) + turretsSize);

  uint8_t* ptr = buffer.data();

  std::memcpy(ptr, &id, sizeof(int));
  ptr += sizeof(int);
  std::memcpy(ptr, &size, sizeof(float));
  ptr += sizeof(float);
  std::memcpy(ptr, &layer, 1);
  ptr += 1;
  std::memcpy(ptr, &shape, 1);
  ptr += 1;
  std::memcpy(ptr, &gs, sizeof(int));
  ptr += sizeof(int);

  for (auto& gun : guns) {
    std::vector<uint8_t> gunData = gun.encode();

    std::memcpy(ptr, gunData.data(), gunData.size());
    ptr += gunData.size();
  }

  std::memcpy(ptr, &ts, sizeof(int));
  ptr += sizeof(int);

  for (auto& turret : turrets) {
    std::vector<uint8_t> turretData = turret.encode();

    std::memcpy(ptr, turretData.data(), turretData.size());
    ptr += turretData.size();
  }

  return buffer;
}

void generateMockups() {
  auto startTime = std::chrono::steady_clock::now();
  int size = 0;

  std::vector<std::vector<uint8_t>> encodedData;

  for (auto& d : Definition::definitions) {
    d.second.id = Definition::counter++;
    std::vector<uint8_t> e = d.second.encode();

    encodedData.push_back(e);

    size += e.size();
  }

  std::vector<uint8_t> mockups(size);
  uint8_t* ptr = mockups.data();

  for (auto& d : encodedData) {
    std::memcpy(ptr, d.data(), d.size());
    ptr += d.size();
  }

  std::ofstream ofs{"mockups.hex"};

  ofs << std::hex << std::setfill('0');
  for (uint8_t u : mockups) {
    ofs << std::setw(2) << (int)u;
  }

  ofs.close();

  auto endTime = std::chrono::steady_clock::now();

  std::cout << size << " bytes of mockups generated in "
            << std::chrono::duration_cast<std::chrono::milliseconds>(endTime -
                                                                     startTime)
                   .count()
            << "ms" << '\n';
}
