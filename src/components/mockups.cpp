#include "mockups.h"
#include <fstream>
#include <iomanip>

Definition aggressor = {.size = 35,
                        .shape = 0,
                        .guns = {{
                            .length = 100,
                            .width = 30,
                            .yOffset = 10,
                        }}};

Definition adfa = {.size = 100, .shape = 9};

std::map<std::string, Definition> Definition::definitions = {
    {"aggressor", aggressor}, {"test", adfa}};

int Definition::counter = 0;

void generateMockups() {
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
}
