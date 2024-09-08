#include "entity.h"
#include <string_view>
#include <uWebSockets/App.h>

struct SocketData {
  std::string color;
  int id;
};
using WS = uWS::WebSocket<false, true, SocketData>;

class Client {
public:
  static std::map<int, Client*> instances;

  Client(WS* socket, int id, std::string color, hshg* grid);
  ~Client();

  void tick();

  void talk(std::string_view message) const;
  void handleMessage(std::string_view message);
  void kick() const;

  int getEntityId() const { return entityId; };
  bool isDead() const { return disconnected; };
  bool playerSpawned() const { return pSpawn; };

  Entity* entity;

private:
  WS* socket;
  bool disconnected;

  int id;
  int entityId;
  bool pSpawn;
  std::string color;
  hshg* grid;

  XY movement;
  XY mouse;

  bool lmb;
  bool rmb;
};
