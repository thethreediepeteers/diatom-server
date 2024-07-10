#include "server.h"
#include <iostream>
#include <uWebSockets/App.h>

int counter = 0;

void Server::run(int port) {
  uWS::App()
      .ws<SocketData>(
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
  SocketData *data = ws->getUserData();
  data->id = counter++;

  std::cout << "Client " << data->id << " connected from "
            << ws->getRemoteAddress() << '\n';
}

void Server::socketMessage(WS *ws, std::string_view message,
                           uWS::OpCode /*opCode*/) {
  SocketData *data = ws->getUserData();
  std::cout << "Message from client " << data->id << " recieved: " << message
            << '\n';
}

void Server::socketClose(WS *ws, int /*code*/, std::string_view /*message*/) {
  SocketData *data = ws->getUserData();
  std::cout << "Client " << data->id << " disconnected" << '\n';
}
