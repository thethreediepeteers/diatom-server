#include "modules/server.h"
#include <csignal>

const int PORT = 3000;
const int FPS = 30;

void gameLoop();

int main() {
  auto loop = uWS::Loop::get();
  us_timer_t* delayTimer = us_create_timer((us_loop_t*)loop, 0, 0);
  us_timer_set(
      delayTimer, [](us_timer_t*) { gameLoop(); }, 1000 / FPS, 1000 / FPS);

  std::signal(SIGINT, Server::cleanup);
  std::signal(SIGTERM, Server::cleanup);

  Server::run(PORT, delayTimer);

  std::cout << "Server successfully shut down" << '\n';

  return 0;
}

void gameLoop() {
  for (auto& client : Client::instances) {
    client.second->tick();
  }

  for (auto& entity : Entity::instances) {
    entity.second->tick();
  }
}
