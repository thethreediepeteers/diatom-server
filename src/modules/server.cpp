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
hshg* grid;

namespace server {
  void run(int port, us_timer_t* timer, hshg* g) {
    delayTimer = timer;
    grid = g;

    uWS::App()
        .ws<SocketData>("/ws",
                        {.upgrade =
                             [](auto* res, auto* req, auto* context) {
                               std::string_view color = req->getQuery("color");

                               res->template upgrade<SocketData>(
                                   {.color = std::string{color}, .id = -1},
                                   req->getHeader("sec-websocket-key"),
                                   req->getHeader("sec-websocket-protocol"),
                                   req->getHeader("sec-websocket-extensions"),
                                   context);
                             },
                         .open = socketOpen,
                         .message = socketMessage,
                         .close = socketClose})
        .get("/mockups",
             [](auto* res, auto* /*req*/) {
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

  void cleanup(int /*signal*/) {
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

    hshg_free(grid);
  }

  int counter = 0;
  void socketOpen(WS* ws) {
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

    Client* client = new Client(ws, id, data->color, grid);

    std::vector<uint8_t> mapData = map.encode();
    std::vector<uint8_t> buffer(1 + mapData.size());

    buffer[0] = 'm'; // header

    std::memcpy(buffer.data() + 1, mapData.data(), mapData.size());

    std::string_view dataView(reinterpret_cast<char*>(buffer.data()),
                              buffer.size());

    client->talk(dataView);

    std::cout << "Client " << id << " connected from " << ip << '\n';
  }

  void socketMessage(WS* ws, std::string_view message, uWS::OpCode opCode) {
    if (opCode != uWS::OpCode::BINARY) {
      return;
    }

    int id = ws->getUserData()->id;

    Client* client = Client::instances[id];
    client->handleMessage(message);
  }

  void socketClose(WS* ws, int code, std::string_view /*message*/) {
    if (config::IP_LIMIT > 0) {
      --ips[ws->getRemoteAddressAsText()];
    }

    if (code == 1008) {
      return;
    }

    int id = ws->getUserData()->id;

    std::cout << "Client " << id << " disconnected" << '\n';

    Client* client = Client::instances[id];
    if (!client) {
      return;
    }

    if (!client->isDead()) {
      Client::instances.erase(id);
      delete client;
    }
  }
} // namespace server
