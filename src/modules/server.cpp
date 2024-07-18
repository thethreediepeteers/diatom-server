#include "server.h"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <websocketpp/close.hpp>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/logger/levels.hpp>
#include <websocketpp/transport/asio/endpoint.hpp>

using nlohmann::json;
using websocketpp::lib::bind;
using namespace websocketpp::lib::placeholders;

Server::WSServer Server::server{};

Server::Server() {
  server.init_asio();

  server.set_open_handler(bind(&Server::socketOpen, this, ::_1));
  server.set_message_handler(bind(&Server::socketMessage, this, ::_1, ::_2));
  server.set_close_handler(bind(&Server::socketClose, this, ::_1));

  server.clear_access_channels(websocketpp::log::alevel::all);
  server.clear_error_channels(websocketpp::log::elevel::all);
}

void Server::run(int port) {
  server.listen(port);
  std::cout << "WebSocket server listening on port " << port << '\n';

  server.start_accept();
  server.run();
}

std::map<websocketpp::connection_hdl, int,
         std::owner_less<websocketpp::connection_hdl>>
    Server::connections{};

int counter = 0;
void Server::socketOpen(WSHandle ws) {
  int id = counter++;
  connections[ws] = id;

  std::shared_ptr conn = server.get_con_from_hdl(ws);

  Client* client = new Client(conn, id);

  json j = {0, {"id", id}};
  client->talk(j.dump());

  std::cout << "Client " << id << " connected from " << conn->get_host()
            << '\n';
}

void Server::socketMessage(WSHandle ws, WSServer::message_ptr message) {
  int id = connections[ws];
  Client* client = Client::instances[id];
  client->handleMessage(message->get_payload());
}

void Server::socketClose(WSHandle ws) {
  int id = connections[ws];
  std::cout << "Client " << id << " disconnected" << '\n';

  Client* client = Client::instances[id];
  delete client;

  connections.erase(ws);
}

void Server::cleanup(int signal) {
  server.stop_listening();

  for (auto c : connections) {
    server.close(c.first, websocketpp::close::status::going_away,
                 "Server shutdown");
  }
  connections.clear();
}
