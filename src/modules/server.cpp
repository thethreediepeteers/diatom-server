#include "server.h"

#include <iostream>
#include <sstream>
#include <string>

int counter = 0;

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

void Server::socketOpen(WS *ws) {
  Connection *data = ws->getUserData();
  data->id = counter++;

  Client *client = new Client(ws, data->id);

  // client->talk(std::to_string(data->id));
  client->talk("Enter name: ");
  std::cout << "Client " << data->id << " connected from "
            << ws->getRemoteAddress() << '\n';
}

void Server::socketMessage(WS *ws, std::string_view message,
                           uWS::OpCode /*opCode*/) {
  Connection *data = ws->getUserData();

  std::string s{message};
  s = Util::trim(s);

  if (data->name == "") {
    data->name = s;
    return;
  }

  Client *client = Client::instances[data->id];
  // client->handleMessage(message);
  std::ostringstream oss;
  oss << data->name << ": " << message << '\n';
  for (auto c : Client::instances)
    c.second->talk(oss.str());
}

void Server::socketClose(WS *ws, int /*code*/, std::string_view /*message*/) {
  Connection *data = ws->getUserData();
  std::cout << "Client " << data->id << " disconnected" << '\n';

  Client *client = Client::instances[data->id];
  delete client;
}
