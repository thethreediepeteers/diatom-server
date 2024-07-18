#include "../modules/util.h"
#include "entity.h"
#include <map>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio.hpp>
#include <websocketpp/connection.hpp>

using WS = std::shared_ptr<websocketpp::connection<websocketpp::config::asio>>;

class Client : Entity {
public:
  static std::map<int, Client*> instances;

  Client(WS socket, int id);
  ~Client();

  void tick();

  void talk(std::string message);
  void handleMessage(std::string message);
  void kick();

private:
  enum class MessageType { Movement };
  WS socket;
  int id;

  XY movement;
};
