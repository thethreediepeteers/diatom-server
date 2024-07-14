#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
template <typename T>
class MessageQueue {
public:
  void send(T message);
  T receive();

private:
  std::queue<T> queue;
  std::mutex m;
  std::condition_variable cv;
};
