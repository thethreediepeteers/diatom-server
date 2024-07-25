#include "../modules/util.h"
#include "entity.h"
#include <uWebSockets/App.h>

struct SocketData {
  int id;
};
using WS = uWS::WebSocket<false, true, SocketData>;

class Client : Entity {
public:
  static std::map<int, Client*> instances;

  Client(WS* socket, int id);
  ~Client();

  void tick();

  void talk(std::string message);
  void handleMessage(std::string message);
  void kick();

private:
  enum class MessageType { Movement };

  WS* socket;
  int id;

  XY movement;
};
