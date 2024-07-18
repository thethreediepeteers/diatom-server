#include "../components/client.h"
#include <map>
#include <memory>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

class Server {
public:
  Server();

  void run(int port);
  static void cleanup(int signal);

private:
  using WSServer = websocketpp::server<websocketpp::config::asio>;
  using WSHandle = websocketpp::connection_hdl;
  static WSServer server;
  static std::map<WSHandle, int, std::owner_less<WSHandle>> connections;

  void socketOpen(WSHandle ws);
  void socketMessage(WSHandle ws, WSServer::message_ptr message);
  void socketClose(WSHandle ws);
};
