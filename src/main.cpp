#include "components/mockups.h"
#include "modules/config.h"
#include "modules/physics.h"
#include "modules/server.h"
#include <csignal>
#include <iostream>

void setupSignals();
void tick();
us_timer_t* setupLoop();

hshg* HSHG;

int main() {
  generateMockups();
  setupSignals();

  HSHG = initHSHG();

  us_timer_t* loop = setupLoop();

  server::run(config::SERVER_PORT, loop, HSHG);

  std::cout << "Server successfully shut down" << '\n';

  return 0;
}

void setupSignals() {
  std::signal(SIGINT, server::cleanup);
  std::signal(SIGTERM, server::cleanup);
}

void tick() {
  hshg_optimize(HSHG);
  hshg_collide(HSHG);

  for (const auto& client : Client::instances) {
    client.second->tick();
  }

  for (const auto& entity : Entity::instances) {
    entity.second->tick();
  }
  for (const auto& id : Entity::toDelete) {
    delete Entity::instances[id];
    Entity::instances.erase(id);
  }
  Entity::toDelete.clear();

  hshg_update(HSHG);
}

us_timer_t* setupLoop() {
  uWS::Loop* loop = uWS::Loop::get();
  us_timer_t* delayTimer = us_create_timer((us_loop_t*)loop, 0, 0);

  us_timer_set(
      delayTimer, [](us_timer_t*) { tick(); }, 1000 / config::SERVER_FPS,
      1000 / config::SERVER_FPS);

  return delayTimer;
}
