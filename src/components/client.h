#pragma once

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

  int getEntityId() const;

  bool isDead() const;
  bool playerSpawned() const;

  Entity* entity;

private:
  int id;
  int entityId;

  bool disconnected;
  bool pSpawn;
  bool lmb;
  bool rmb;
  
  std::string color;
  hshg* grid;
  WS* socket;

  XY movement;
  XY mouse;
};
