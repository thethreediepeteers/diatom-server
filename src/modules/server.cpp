#include "server.h"
#include "../config.h"

#include <nlohmann/json.hpp>

using nlohmann::json;

Server::WSServer server;
std::map<Server::WSHandle, int, std::owner_less<Server::WSHandle>> connections;

void Server::run(int port) {
  server.init_asio();

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
  server.run();
}

int counter = 0;
void Server::socketOpen(WSHandle ws) {
  int id = counter++;
  connections[ws] = id;

  std::shared_ptr conn = server.get_con_from_hdl(ws);

  Client *client = new Client(conn, id);

  json j = {0, {{"id", id}}};
  client->talk(j.dump());

  std::cout << "Client " << id << " connected from " << conn->get_host()
            << '\n';
}

void Server::socketMessage(WSHandle ws, WSServer::message_ptr message) {
  int id = connections[ws];
  Client *client = Client::instances[id];
  client->handleMessage(message->get_payload());
}

void Server::socketClose(WSHandle ws) {
  int id = connections[ws];

  std::cout << "Client " << id << " disconnected" << '\n';

  Client *client = Client::instances[id];
  delete client;

  connections.erase(ws);
}

void Server::cleanup(int signal) {
  server.stop_listening();

  std::vector<WSHandle> conns;

  for (auto c : connections) {
    conns.push_back(c.first);
  }

  for (WSHandle handle : conns) {
    server.close(handle, websocketpp::close::status::going_away,
                 "Server shutdown");
  }

  connections.clear();
}
