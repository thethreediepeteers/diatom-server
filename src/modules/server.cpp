#include "server.h"
#include "../config.h"

#include <nlohmann/json.hpp>

using nlohmann::json;

us_listen_socket_t* listenSocket;
us_timer_t* delayTimer;

void Server::run(int port, us_timer_t* timer) {
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

  /*server.init_asio();

  server.set_open_handler(Server::socketOpen);
  server.set_message_handler(Server::socketMessage);
  server.set_close_handler(Server::socketClose);

  server.set_reuse_addr(true);

  if (!config::ENABLE_SERVER_LOGGING) {
    server.clear_access_channels(websocketpp::log::alevel::all);
    server.clear_error_channels(websocketpp::log::elevel::all);
  }

  server.listen(port);
  std::cout << "WebSocket server listening on port " << port << '\n';

  server.start_accept();
  server.run();*/
}

int counter = 0;
void Server::socketOpen(WS* ws) {
  int id = counter++;

  SocketData* data = ws->getUserData();
  data->id = id;

  Client* client = new Client(ws, id);

  json j = {0, {{"id", id}}};
  client->talk(j.dump());

  std::cout << "Client " << id << " connected from "
            << ws->getRemoteAddressAsText() << '\n';
}

void Server::socketMessage(WS* ws, std::string_view message,
                           uWS::OpCode /*opCode*/) {
  int id = ws->getUserData()->id;

  Client* client = Client::instances[id];
  client->handleMessage(std::string{message});
}

void Server::socketClose(WS* ws, int /*code*/, std::string_view /*message*/) {
  int id = ws->getUserData()->id;

  std::cout << "Client " << id << " disconnected" << '\n';

  Client* client = Client::instances[id];
  if (client) {
    Client::instances.erase(id);
    delete client;
  }
}

void Server::cleanup(int signal) {
  std::cout << '\n';

  for (auto it = Client::instances.begin(); it != Client::instances.end();) {
    Client* client = it->second;
    it = Client::instances.erase(it);
    delete client;
  }

  us_listen_socket_close(0, listenSocket);
  us_timer_close(delayTimer);
}
