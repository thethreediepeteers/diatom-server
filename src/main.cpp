#include "modules/config.h"
#include "modules/physics.h"
#include "modules/server.h"
#include <csignal>

const int PORT = 3000;
const int FPS = 30;

void setupSignals();
void tick();
us_timer_t* setupLoop();

int main() {
  /*
  for (int i = 0; i < 100; ++i) {
    auto n =
        new Entity(util::randint(config::MAP_WIDTH),
                   util::randint(config::MAP_HEIGHT), util::randint(35, 70));
  }
       */
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
