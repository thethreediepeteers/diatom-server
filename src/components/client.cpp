#include "client.h"
#include "../modules/config.h"
#include <cmath>
#include <iostream>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

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

  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartArray();
  writer.Int(1);

  for (auto& e : Entity::instances) {
    auto doc = e.second->encode();
    doc.Accept(writer);
  }
  writer.EndArray();

  talk(s.GetString());
}

void Client::talk(std::string message) {
  socket->send(message, uWS::OpCode::TEXT);
}
void Client::handleMessage(std::string message) {
  util::trim(message);
  std::cout << "Message from client " << id << " received: " << message << '\n';

  rapidjson::Document doc;
  doc.Parse(message.c_str());

  if (doc.Size() == 3 && doc[0].GetInt() == 0 && doc[1].IsNumber() &&
      doc[2].IsNumber()) { // movement packet
    double m = doc[1].GetDouble();
    bool moving = doc[2].GetInt() & 1;
    movement = moving ? XY(std::cos(m), std::sin(m)) : XY(0, 0);
  }
}
void Client::kick() {
  instances.erase(id);
  socket->close();

  std::cout << "Client " << id << " kicked" << '\n';
}
