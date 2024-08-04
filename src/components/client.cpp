#include "client.h"
#include "modules/config.h"
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <numbers>
#include <vector>

std::map<int, Client*> Client::instances{};

Client::Client(WS* socket, int id)
    : socket(socket), id(id), disconnected(false),
      Entity(util::rand(config::MAP_WIDTH), util::rand(config::MAP_HEIGHT),
             util::rand<float>(35, 75), util::rand<float>(0, std::numbers::pi),
             util::rand<uint8_t>(3, 15), util::randcolor()),
      movement(XY(0, 0)), mouse(XY(0, 0)) {
  instances[id] = this;
  entityId = this->getId();
};

Client::~Client() {
  Entity::instances.erase(entityId);
  disconnected = true;
  socket->close();
}

void Client::tick() {
  vel += movement;

  size_t entitySize = sizeof(int) + sizeof(double) * 2 + sizeof(float) * 2 +
                      4; // 4: shape (1), color (3)
  std::vector<uint8_t> buffer(Entity::instances.size() * entitySize);
  uint8_t* ptr = buffer.data();

  for (auto& e : Entity::instances) {
    std::vector<uint8_t> state = e.second->encode();
    memcpy(ptr, state.data(), state.size());
    ptr += entitySize;
  }

  std::string_view dataView(reinterpret_cast<char*>(buffer.data()),
                            buffer.size());
  talk(dataView);
}

void Client::talk(std::string_view message) const {
  socket->send(message, uWS::OpCode::BINARY);
}
void Client::handleMessage(std::string_view message) {
  if (message.size() != 2 * sizeof(short) + sizeof(float) + sizeof(int))
    return;

  try {
    short mx;
    short my;
    float m;
    int flags;

    auto ptr = message.data();

    memcpy(&mx, ptr, sizeof(short));
    ptr += sizeof(short);

    memcpy(&my, ptr, sizeof(short));
    ptr += sizeof(short);

    memcpy(&m, ptr, sizeof(float));
    ptr += sizeof(float);

    memcpy(&flags, ptr, sizeof(int));

    bool moving = flags & 1;
    bool lmb = flags & 2;

    movement = moving ? XY(std::cos(m), std::sin(m)) : XY(0, 0);

    mouse = XY(mx, my);
    angle = atan2(my, mx);
  } catch (...) {
  }
}
void Client::kick() const {
  instances.erase(id);
  socket->close();

  std::cout << "Client " << id << " kicked" << '\n';
}
