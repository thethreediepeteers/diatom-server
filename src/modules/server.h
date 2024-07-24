#include "../components/client.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

namespace Server {
void run(int port);
void cleanup(int signal);

using WSServer = websocketpp::server<websocketpp::config::asio>;
using WSHandle = websocketpp::connection_hdl;

void socketOpen(WSHandle ws);
void socketMessage(WSHandle ws, WSServer::message_ptr message);
void socketClose(WSHandle ws);
} // namespace Server
