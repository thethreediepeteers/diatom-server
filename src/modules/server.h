#include <uWebSockets/App.h>

namespace Server {
struct SocketData {
  int id;
};

typedef uWS::WebSocket<false, true, SocketData> WS;

void run(int port);

void socketOpen(WS *ws);
void socketMessage(WS *ws, std::string_view message, uWS::OpCode /*opCode*/);
void socketClose(WS *ws, int /*code*/, std::string_view /*message*/);
} // namespace Server
