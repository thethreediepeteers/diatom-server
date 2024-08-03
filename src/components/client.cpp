#include "client.h"
#include "modules/config.h"
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>

std::map<int, Client*> Client::instances{};

Client::Client(WS* socket, int id)
    : socket(socket), id(id), disconnected(false),
      Entity(util::rand(config::MAP_WIDTH), util::rand(config::MAP_HEIGHT),
             util::rand<float>(100),
             util::HexColor(util::rand<uint8_t>(255), util::rand<uint8_t>(255),
                            util::rand<uint8_t>(255))),
      movement(XY(0, 0)) {
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

  size_t entitySize =
      sizeof(int) + sizeof(double) * 2 + sizeof(float) + 3; // 3 for color
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
  if (message.size() != sizeof(float) + sizeof(int))
    return;

  try {
    float m;
    int flags;

    auto ptr = message.data();

    memcpy(&m, ptr, sizeof(float));
    ptr += sizeof(float);

    memcpy(&flags, ptr, sizeof(int));

    bool moving = flags & 1;
    bool lmb = flags & 2;

    movement = moving ? XY(std::cos(m), std::sin(m)) : XY(0, 0);
  } catch (...) {
  }
}
void Client::kick() const {
  instances.erase(id);
  socket->close();

  std::cout << "Client " << id << " kicked" << '\n';
}
