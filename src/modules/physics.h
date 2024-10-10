#pragma once

#define HSHG_D 2
#define HSHG_UNIFORM

#include "hshg/hshg.h"
#include <cstdint>
#include <vector>

struct XY {
  double x, y;

  XY(double x, double y) : x(x), y(y) {}

  XY operator*(double n) { return {x * n, y * n}; }
  XY operator/(double n) { return {x / n, y / n}; }

  void operator+=(const XY other) {
    x += other.x;
    y += other.y;
  }
  void operator*=(double n) {
    x *= n;
    y *= n;
  }
};

struct Map {
  int width, height;

  Map(int width, int height);

  std::vector<uint8_t> encode() const;
};

hshg* initHSHG();

void updateHSHG(hshg* grid, hshg_entity* entity);
void collideHSHG(const hshg* hshg, const hshg_entity* a, const hshg_entity* b);
