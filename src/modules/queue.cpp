#include "queue.h"
#include <mutex>

template <typename T>
void MessageQueue<T>::send(T message) {
  std::lock_guard<std::mutex> lock(m);
  queue.push(std::move(message));

  cv.notify_one();
}

template <typename T>
T MessageQueue<T>::receive() {
  std::unique_lock<std::mutex> lock(m);
  cv.wait(lock, [this] { return !queue.empty(); });
  T message = std::move(queue.front());
  queue.pop();
  return message;
}
