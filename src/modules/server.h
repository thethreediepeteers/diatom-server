#include "components/client.h"
#include <uWebSockets/App.h>

namespace server {
  void run(int port, us_timer_t* timer);
  void cleanup(int signal);

  void socketOpen(WS* ws);
  void socketMessage(WS* ws, std::string_view message, uWS::OpCode opCode);
  void socketClose(WS* ws, int code, std::string_view message);
} // namespace server
