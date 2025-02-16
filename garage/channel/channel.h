#include <iostream>

namespace garage {
template <typename T>
class channel {
 public:
  channel(int size) : size_(size) {};
  ~channel() = default;

  void send(T data) {
    std::unique_lock lock(mutex_);
    while (queue_.size() == size_) {
      cv_.wait(lock);
    }
    queue_.push(data);
    cv_.notify_all();
  }

  T recv() {
    std::unique_lock lock(mutex_);
    while (queue_.empty()) {
      cv_.wait(lock);
    }
    T x = queue_.front();
    queue_.pop();
    cv_.notify_all();
    return x;
  };

  friend channel& operator<<(channel& target, const T data) {
    target.send(data);
    return target;
  };

 private:
  int size_;
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cv_;
};
}  // namespace garage
