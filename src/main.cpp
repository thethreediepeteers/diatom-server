#include "modules/server.h"

const int PORT = 3000;
const int FPS = 30;

void gameLoop();

int main() {
  std::thread loop(gameLoop);
  std::signal(SIGINT, Server::cleanup);
  std::signal(SIGTERM, Server::cleanup);

  Server::run(PORT);

  loop.detach(); // when server is done running

  std::cout << "Server successfully shut down" << '\n';

  return 0;
}

void gameLoop() {
  bool quit = false;

  const std::chrono::milliseconds frameDuration(1000 / FPS);

  while (!quit) {
    auto start = std::chrono::steady_clock::now();

    {
      for (auto &client : Client::instances) {
        client.second->tick();
      }

      for (auto &entity : Entity::instances) {
        entity.second->tick();
      }
    }

    auto end = std::chrono::steady_clock::now();
    auto loopTime = end - start;

    if (loopTime < frameDuration) {
      std::this_thread::sleep_for(frameDuration - loopTime);
    }
  }
}
