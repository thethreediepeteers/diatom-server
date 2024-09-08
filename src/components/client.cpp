#include "client.h"
#include "modules/config.h"
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>

std::map<int, Client*> Client::instances{};

Client::Client(WS* socket, int id, std::string color, hshg* grid)
    : entity(nullptr), socket(socket), disconnected(false), id(id),
      entityId(-1), pSpawn(false), color(color), grid(grid), movement(XY(0, 0)),
      mouse(XY(0, 0)), lmb(false), rmb(false) {
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
  if (!entity) {
    return;
  }
  entity->vel += movement;

  if (lmb) {
    entity->shoot();
  }

  size_t entitySize = sizeof(int) * 5 + sizeof(double) * 2 + sizeof(float) * 2 +
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
  if (message.size() == sizeof(int)) {
    // spawn request

    if (pSpawn) {
      kick(); // kick if already spawned
    }

    entity = new Entity(util::rand(config::MAP_WIDTH),
                        util::rand(config::MAP_HEIGHT), 0,
                        util::rand<uint8_t>(3, 15), color, grid);
    entityId = entity->getId();
    entity->spawn("aggressor");

    std::vector<uint8_t> buffer(sizeof(int));
    uint8_t* ptr = buffer.data();
    memcpy(ptr, &entityId, sizeof(int));
    ptr += sizeof(int);

    std::string_view dataView(reinterpret_cast<char*>(buffer.data()),
                              buffer.size());
    talk(dataView);
  }

  else if (message.size() == 2 * sizeof(short) + sizeof(float) + sizeof(int)) {
    // tick message

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

  std::cout << "Client " << id << " kicked" << '\n';
}
