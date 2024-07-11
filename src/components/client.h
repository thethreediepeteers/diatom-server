#include "../modules/util.h"
#include "entity.h"
#include <map>
#include <uWebSockets/App.h>

struct Connection {
  int id;
  std::string name;
};

using WS = uWS::WebSocket<false, true, Connection>;

class Client : Entity {
public:
  static std::map<int, Client *> instances;

  Client(WS *socket, int id);
  ~Client();

  void talk(std::string_view message);
  void handleMessage(std::string_view message);
  void kick();

private:
  WS *socket;
  int id;
};
