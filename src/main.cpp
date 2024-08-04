#include "modules/config.h"
#include "modules/server.h"
#include <csignal>
#include <cstdint>
#include <numbers>

const int PORT = 3000;
const int FPS = 30;

void setupSignals();
void tick();
us_timer_t* setupLoop();

int main() {
  /*for (int i = 0; i < 1000; ++i) {
    new Entity(util::rand<int>(config::MAP_WIDTH),
               util ::rand<int>(config::MAP_HEIGHT), util::rand<float>(35, 75),
               util::rand<float>(std::numbers::pi), util::rand<uint8_t>(3, 15),
               util::randcolor());
  }*/
  setupSignals();
  server::run(PORT, setupLoop());

  std::cout << "Server successfully shut down" << '\n';

  return 0;
}

void setupSignals() {
  std::signal(SIGINT, server::cleanup);
  std::signal(SIGTERM, server::cleanup);
}

void tick() {
  for (auto& client : Client::instances) {
    client.second->tick();
  }

  for (auto& entity : Entity::instances) {
    entity.second->tick();
    entity.second->stayInBounds(0, 0, config::MAP_WIDTH, config::MAP_HEIGHT);
  }
}

us_timer_t* setupLoop() {
  auto loop = uWS::Loop::get();
  us_timer_t* delayTimer = us_create_timer((us_loop_t*)loop, 0, 0);
  us_timer_set(delayTimer, [](us_timer_t*) { tick(); }, 1000 / FPS, 1000 / FPS);

  return delayTimer;
}
