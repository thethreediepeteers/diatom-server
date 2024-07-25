#include "client.h"
#include <cmath>
#include <iostream>
#include <nlohmann/json.hpp>
#include <uWebSockets/WebSocketProtocol.h>

using nlohmann::json;

std::map<int, Client*> Client::instances{};

Client::Client(WS* socket, int id)
    : socket(socket), id(id), Entity(0, 0), movement(XY(0, 0)) {
  instances[id] = this;
};
Client::~Client() { socket->close(); }

void Client::tick() {
  vel += movement;

  json message;

  message.push_back(1);
  for (auto& e : Entity::instances) {
    message.push_back(e.second->encode());
  }

  talk(message.dump());
}

void Client::talk(std::string message) {
  socket->send(message, uWS::OpCode::TEXT);
}
void Client::handleMessage(std::string message) {
  Util::trim(message);
  std::cout << "Message from client " << id << " received: " << message << '\n';

  try {
    json j = json::parse(message);

    if (j.size() == 3 && j[0] == MessageType::Movement && j[1].is_number() &&
        j[2].is_number()) { // movement packet

      double m = j[1];
      bool moving = j[2].get<int>() & 1;
      movement = moving ? XY(std::cos(m), std::sin(m)) : XY(0, 0);
    }
  } catch (...) {
  } // invalid packet received*/
}
void Client::kick() {
  instances.erase(id);
  socket->close();

  std::cout << "Client " << id << " kicked" << '\n';
}
