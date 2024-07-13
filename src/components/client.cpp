#include "client.h"
#include <cmath>
#include <iostream>
#include <nlohmann/json.hpp>
#include <uWebSockets/Loop.h>
#include <uWebSockets/WebSocketProtocol.h>

using nlohmann::json;

std::map<int, Client *> Client::instances{};

Client::Client(WS *socket, int id)
    : socket(socket), id(id), Entity(0, 0), movement(XY(0, 0)) {
  instances[id] = this;
};
Client::~Client() { instances.erase(id); }

void Client::tick() {
  getVel() += movement;

  json message;

  message.push_back(1);
  for (auto &e : Entity::instances) {
    message.push_back(e.second->encode());
  }

  talk(message.dump());
}

void Client::talk(std::string_view message) {
  socket->send(message, uWS::OpCode::TEXT);
}
void Client::handleMessage(std::string_view message) {
  try {
    json j = json::parse(message);

    std::cout << "Message from client " << id << " received: " << j << '\n';
    if (j.size() == 3 && j[0] == 0 && j[1].is_number() &&
        j[2].is_boolean()) { // movement packet
      if (j[2]) {
        double m = j[1];
        movement = {std::cos(m), std::sin(m)};
      } else {
        movement = {0, 0};
      }
    }
  } catch (...) { // invalid packet received
  }
}
void Client::kick() {
  socket->close();
  std::cout << "Client " << id << " kicked" << '\n';
  delete this;
}
