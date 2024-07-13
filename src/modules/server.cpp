#include "server.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using nlohmann::json;

void Server::run(int port) {
  uWS::App()
      .ws<Connection>(
          "/ws",
          {.open = socketOpen, .message = socketMessage, .close = socketClose})
      .listen(port,
              [port](auto *listenSocket) {
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
void Server::socketOpen(WS *ws) {
  Connection *data = ws->getUserData();
  data->id = counter++;

  Client *client = new Client(ws, data->id);

  json j = {0, {"id", data->id}};
  client->talk(j.dump());
  std::cout << "Client " << data->id << " connected from "
            << ws->getRemoteAddressAsText() << '\n';
}

void Server::socketMessage(WS *ws, std::string_view message,
                           uWS::OpCode /*opCode*/) {
  Connection *data = ws->getUserData();

  Client *client = Client::instances[data->id];
  client->handleMessage(message);
}

void Server::socketClose(WS *ws, int /*code*/, std::string_view /*message*/) {
  Connection *data = ws->getUserData();
  std::cout << "Client " << data->id << " disconnected" << '\n';

  Client *client = Client::instances[data->id];
  delete client;
}
