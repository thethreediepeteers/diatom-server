#include "server.h"
#include "config.h"
#include "physics.h"
#include <libusockets.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

us_listen_socket_t* listenSocket;
us_timer_t* delayTimer;
Map map(config::MAP_WIDTH, config::MAP_HEIGHT);

void server::run(int port, us_timer_t* timer) {
  delayTimer = timer;

  uWS::App()
      .ws<SocketData>(
          "/ws",
          {.open = socketOpen, .message = socketMessage, .close = socketClose})
      .listen(port,
              [port](auto* token) {
                listenSocket = token;
                if (listenSocket) {

                  std::cout << "WebSocket server listening on port " << port
                            << '\n';
                } else {
                  std::cout << "Failed to bind to port " << port << '\n';
                }
              })
      .run();
}

int counter = 0;
void server::socketOpen(WS* ws) {
  int id = counter++;

  SocketData* data = ws->getUserData();
  data->id = id;

  Client* client = new Client(ws, id);

  rapidjson::StringBuffer s;
  rapidjson::Writer<rapidjson::StringBuffer> writer(s);

  writer.StartArray();
  writer.Int(0);
  writer.StartObject();
  writer.Key("id");
  writer.Int(client->getEntityId());
  writer.Key("map");

  auto doc = map.encode();
  doc.Accept(writer);

  writer.EndObject();
  writer.EndArray();

  client->talk(s.GetString());

  std::cout << "Client " << id << " connected from "
            << ws->getRemoteAddressAsText() << '\n';
}

void server::socketMessage(WS* ws, std::string_view message,
                           uWS::OpCode /*opCode*/) {
  int id = ws->getUserData()->id;

  Client* client = Client::instances[id];
  client->handleMessage(std::string{message});
}

void server::socketClose(WS* ws, int /*code*/, std::string_view /*message*/) {
  int id = ws->getUserData()->id;

  std::cout << "Client " << id << " disconnected" << '\n';

  Client* client = Client::instances[id];
  if (!client->isDead()) {
    Client::instances.erase(id);
    delete client;
  }
}

void server::cleanup(int signal) {
  std::cout << '\n';

  us_timer_close(delayTimer);
  us_listen_socket_close(0, listenSocket);

  for (auto c : Client::instances) {
    delete c.second;
  }
  Client::instances.clear();

  for (auto e : Entity::instances) {
    delete e.second;
  }
  Entity::instances.clear();
}
