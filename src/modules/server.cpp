#include "server.h"
#include "config.h"
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <string_view>

us_listen_socket_t* listenSocket;
us_timer_t* delayTimer;
Map map(config::MAP_WIDTH, config::MAP_HEIGHT);
std::map<std::string_view, int> ips;

void server::run(int port, us_timer_t* timer) {
  delayTimer = timer;

  uWS::App()
      .ws<SocketData>("/ws",
                      {.upgrade =
                           [](auto* res, auto* req, auto* context) {
                             std::string_view color = req->getQuery("color");

                             res->template upgrade<SocketData>(
                                 {.color = std::string{color}},
                                 req->getHeader("sec-websocket-key"),
                                 req->getHeader("sec-websocket-protocol"),
                                 req->getHeader("sec-websocket-extensions"),
                                 context);
                           },
                       .open = socketOpen,
                       .message = socketMessage,
                       .close = socketClose})
      .get("/mockups",
           [](auto* res, auto* req) {
             std::string data;
             std::ifstream ifs{"mockups.hex"};
             ifs >> data;

             res->writeHeader("Access-Control-Allow-Origin", "*");
             res->end(data);
           })
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
  std::string_view ip = ws->getRemoteAddressAsText();

  if (config::IP_LIMIT > 0) {
    ++ips[ip];
    if (ips[ip] > config::IP_LIMIT) {
      ws->end(1008, "IP limit");
      return;
    }
  }

  int id = counter++;

  SocketData* data = ws->getUserData();
  data->id = id;

  std::string color = data->color;
  if (color.size() != 7 || color[0] != '#') {
    ws->end(1008, "Invalid color");
    return;
  }

  Client* client = new Client(ws, id, data->color);

  std::vector<uint8_t> buffer(3 * sizeof(int));
  uint8_t* ptr = buffer.data();

  int entityId = client->getEntityId();
  memcpy(ptr, &entityId, sizeof(int));
  ptr += sizeof(int);

  std::vector<uint8_t> m = map.encode();

  memcpy(ptr, m.data(), m.size());

  std::string_view dataView(reinterpret_cast<char*>(buffer.data()),
                            buffer.size());

  client->talk(dataView);

  std::cout << "Client " << id << " connected from " << ip << '\n';
}

void server::socketMessage(WS* ws, std::string_view message,
                           uWS::OpCode opCode) {
  if (opCode != uWS::OpCode::BINARY)
    return;

  int id = ws->getUserData()->id;

  Client* client = Client::instances[id];
  client->handleMessage(message);
}

void server::socketClose(WS* ws, int code, std::string_view /*message*/) {
  if (config::IP_LIMIT > 0)
    --ips[ws->getRemoteAddressAsText()];
  if (code == 1008)
    return;

  int id = ws->getUserData()->id;

  std::cout << "Client " << id << " disconnected" << '\n';

  Client* client = Client::instances[id];
  if (!client)
    return;
  if (!client->isDead()) {
    Client::instances.erase(id);
    delete client;
  }
}

void server::cleanup(int signal) {
  std::cout << '\n';

  std::filesystem::remove("mockups.hex");
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
