#include "mockups.h"
#include "modules/util.h"
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <lua.hpp>

std::vector<uint8_t> buffer;

enum TypeId : uint8_t {
  TYPE_NUM = 0x01,
  TYPE_STRING = 0x02,
  TYPE_STARTTABLE = 0x03,
  TYPE_ENDTABLE = 0x04,
  TYPE_ARRAY = 0x05,
};

void iterLuaTable(lua_State* L) {
  lua_pushnil(L);

  while (lua_next(L, -2)) {
    if (lua_isinteger(L, -2)) {
      buffer.push_back(TYPE_ARRAY);
    } else if (lua_isstring(L, -2)) {
      std::string key = lua_tostring(L, -2);

      buffer.push_back(TYPE_STRING);
      util::writeStringToBuffer(buffer, key);
    }

    if (lua_isnumber(L, -1)) {
      buffer.push_back(TYPE_NUM);

      double value = lua_tonumber(L, -1);
      util::writeToBuffer(buffer, value);
    } else if (lua_isstring(L, -1)) {
      buffer.push_back(TYPE_STRING);

      std::string value = lua_tostring(L, -1);
      util::writeStringToBuffer(buffer, value);

    } else if (lua_istable(L, -1)) {
      buffer.push_back(TYPE_STARTTABLE);

      iterLuaTable(L);

      buffer.push_back(TYPE_ENDTABLE);
    }

    lua_pop(L, 1);
  }
}

void loadLuaMockups() {
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);

  if (luaL_dofile(L, "definitions.lua") != LUA_OK) {
    std::cout << "Error reading definitions: " << lua_tostring(L, -1) << '\n';
    lua_close(L);
    return;
  }

  lua_getglobal(L, "Exports");

  iterLuaTable(L);
  lua_pop(L, 1);

  lua_close(L);
}

Definition bullet = {.size = 20, .shape = 0, .body = {.health = 10}};

Definition aggressor = {
    .size = 32.5,
    .shape = 0,
    .guns = {{.length = 30,
              .width = 17.5,
              .aspect = 1,
              .body = {.bspeed = 2.5, .reload = 10, .life = 50}}},
    .body = {.health = 50, .speed = 2}};

std::map<std::string, Definition> Definition::definitions = {
    {"bullet", bullet}, {"aggressor", aggressor}};

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

  std::vector<uint8_t> buffer(sizeof(int) + sizeof(float) + 1 + sizeof(int) +
                              gunsSize + sizeof(int) + turretsSize);

  uint8_t* ptr = buffer.data();

  std::memcpy(ptr, &id, sizeof(int));
  ptr += sizeof(int);
  std::memcpy(ptr, &size, sizeof(float));
  ptr += sizeof(float);
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
  for (uint8_t u : buffer) {
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
