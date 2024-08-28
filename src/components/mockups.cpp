#include "mockups.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

Definition base = {.size = 20, .shape = 0};

Definition aggressor = {
    .size = 32.5,
    .shape = 0,
    .guns = {{.length = 30,
              .width = 20,
              .aspect = 1,
              .body = {.bspeed = 2.5, .reload = 10, .life = 50}}}};

std::map<std::string, Definition> Definition::definitions = {
    {"base", base}, {"aggressor", aggressor}};

int Definition::counter = 0;

void generateMockups() {
  auto startTime = std::chrono::steady_clock::now();
  int size = 0;
  std::vector<std::vector<uint8_t>> encodedData;
  for (auto& d : Definition::definitions) {
    d.second.id = Definition::counter++;
    auto e = d.second.encode();
    encodedData.push_back(e);
    size += e.size();
  }

  std::vector<uint8_t> mockups(size);
  uint8_t* ptr = mockups.data();

  for (auto& d : encodedData) {
    memcpy(ptr, d.data(), d.size());
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
