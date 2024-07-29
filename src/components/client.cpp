#include "client.h"
#include "../modules/config.h"
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string_view>
#include <uWebSockets/WebSocketProtocol.h>
#include <vector>

std::map<int, Client*> Client::instances{};

Client::Client(WS* socket, int id)
    : socket(socket), id(id), disconnected(false),
      Entity(util::randint(config::MAP_WIDTH),
             util::randint(config::MAP_HEIGHT), util::randint(100)),
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

  size_t entitySize = sizeof(int) + sizeof(double) * 2 + sizeof(float);
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

void Client::talk(std::string_view message) {
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

    movement = moving ? XY(std::cos(m), std::sin(m)) : XY(0, 0);
  } catch (...) {
  } // message invalid
  //
  /*util::trim(message);
   std::cout << "Message from client " << id << " received: " << message <<
   '\n';

   rapidjson::Document doc;
   doc.Parse(message.c_str());

   if (doc.Size() == 3 && doc[0].GetInt() == 0 && doc[1].IsNumber() &&
       doc[2].IsNumber()) { // movement packet
     double m = doc[1].GetDouble();
     bool moving = doc[2].GetInt() & 1;
     movement = moving ? XY(std::cos(m), std::sin(m)) : XY(0, 0);
   }*/
}
void Client::kick() {
  instances.erase(id);
  socket->close();

  std::cout << "Client " << id << " kicked" << '\n';
}
