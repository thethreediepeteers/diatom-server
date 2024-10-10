#include "client.h"
#include "modules/config.h"
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>

std::map<int, Client*> Client::instances{};

Client::Client(WS* socket, int id, std::string color, hshg* grid)
    : entity(nullptr), id(id), entityId(-1), disconnected(false), pSpawn(false),
      lmb(false), rmb(false), color(color), grid(grid), socket(socket),
      movement(XY(0, 0)), mouse(XY(0, 0)) {
  instances[id] = this;
};

Client::~Client() {
  if (entity) {
    delete Entity::instances[entityId];
    Entity::instances.erase(entityId);
  }

  disconnected = true;
  socket->close();
}

void Client::tick() {
  if (entity) {
    entity->vel += movement;

    if (lmb) {
      entity->shoot();
    }
  }

  size_t entitySize = sizeof(int) * 5 + sizeof(double) * 2 + sizeof(float) * 2 +
                      4; // 4: shape (1), color (3)
  std::vector<uint8_t> buffer(1 + Entity::instances.size() * entitySize);
  uint8_t* ptr = buffer.data() + 1;

  buffer[0] = 'u'; // header

  for (auto& e : Entity::instances) {
    std::vector<uint8_t> state = e.second->encode();

    std::memcpy(ptr, state.data(), state.size());
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
  if (message.size() < 1) {
    return;
  }
  if (message.size() == sizeof(int)) {
    if (pSpawn) {
      kick(); // kick if already spawned
    }

    entity = new Entity(util::rand(config::MAP_WIDTH),
                        util::rand(config::MAP_HEIGHT), 0,
                        util::rand<uint8_t>(3, 15), color, grid);
    entityId = entity->getId();

    entity->spawn("aggressor");

    std::vector<uint8_t> buffer(1 + sizeof(int));
    uint8_t* ptr = buffer.data() + 1;

    buffer[0] = 's'; // header

    std::memcpy(ptr, &entityId, sizeof(int));
    ptr += sizeof(int);

    std::string_view dataView(reinterpret_cast<char*>(buffer.data()),
                              buffer.size());
    talk(dataView);
  }

  else if (message.size() == 2 * sizeof(short) + sizeof(float) + sizeof(int)) {
    short mx;
    short my;
    float m;
    int flags;

    const char* ptr = message.data();

    std::memcpy(&mx, ptr, sizeof(short));
    ptr += sizeof(short);
    std::memcpy(&my, ptr, sizeof(short));
    ptr += sizeof(short);
    std::memcpy(&m, ptr, sizeof(float));
    ptr += sizeof(float);
    std::memcpy(&flags, ptr, sizeof(int));

    bool moving = flags & 1;
    lmb = flags & 2;
    rmb = flags & 4;

    movement = moving ? XY(std::cos(m), std::sin(m)) : XY(0, 0);
    mouse = XY(mx, my);
    entity->angle = atan2(my, mx);
  }
}

void Client::kick() const {
  instances.erase(id);
  socket->close();

  std::cout << "Client " << id << " kicked" << std::endl;
}
