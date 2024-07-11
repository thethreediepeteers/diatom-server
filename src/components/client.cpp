#include "client.h"

std::map<int, Client *> Client::instances{};

Client::Client(WS *socket, int id) : socket(socket), id(id), Entity(0, 0) {
  instances[id] = this;
};
Client::~Client() { instances.erase(id); }

void Client::talk(std::string_view message) { socket->send(message); }
void Client::handleMessage(std::string_view message) {
  std::string m{message};
  std::cout << "Message from client " << id << " recieved: " << Util::trim(m)
            << '\n';
  talk(message);
}
void Client::kick() {
  socket->close();
  std::cout << "Client " << id << " kicked" << '\n';
  delete this;
}
