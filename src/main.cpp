#include "components/mockups.h"
#include "modules/config.h"
#include "modules/physics.h"
#include "modules/server.h"
#include <chrono>
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

  std::cout << "Server successfully shut down" << std::endl;

  return 0;
}

void setupSignals() {
  std::signal(SIGINT, server::cleanup);
  std::signal(SIGTERM, server::cleanup);
}

void tick() {
  auto startt = std::chrono::steady_clock::now();

  auto start1 = std::chrono::steady_clock::now();
  for (const auto& client : Client::instances) {
    client.second->tick();
  }
  auto end1 = std::chrono::steady_clock::now() - start1;

  auto start2 = std::chrono::steady_clock::now();
  for (const auto& id : Entity::toDelete) {
    delete Entity::instances[id];
    Entity::instances.erase(id);
  }
  auto end2 = std::chrono::steady_clock::now() - start2;

  auto start3 = std::chrono::steady_clock::now();
  for (const auto& entity : Entity::instances) {
    entity.second->tick();
  }
  auto end3 = std::chrono::steady_clock::now() - start3;

  auto start4 = std::chrono::steady_clock::now();
  hshg_update(HSHG);
  // hshg_collide(HSHG);
  auto end4 = std::chrono::steady_clock::now() - start4;

  auto endt = std::chrono::steady_clock::now() - startt;

  std::cout
      << "Client tick time: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(end1).count()
      << " ms" << '\n';
  std::cout
      << "Entity delete time: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(end2).count()
      << " ms" << '\n';
  std::cout
      << "Entity tick time: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(end3).count()
      << " ms" << '\n';
  std::cout
      << "HSHG update/collide time: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(end4).count()
      << " ms" << '\n';
  std::cout
      << "Total tick time: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(endt).count()
      << " ms" << '\n';
}

us_timer_t* setupLoop() {
  uWS::Loop* loop = uWS::Loop::get();
  us_timer_t* delayTimer = us_create_timer((us_loop_t*)loop, 0, 0);

  us_timer_set(
      delayTimer, [](us_timer_t*) { tick(); }, 1000 / config::SERVER_FPS,
      1000 / config::SERVER_FPS);

  return delayTimer;
}
