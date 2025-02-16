#include <iostream>

namespace garage {

class executor {
  typedef std::function<void(executor*)> runner;

 public:
  void go(runner func) { runnable_.push(func); };

  void sched(runner callback) { runnable_.push(callback); };

  void run() {
    while (!runnable_.empty()) {
      auto task = runnable_.front();
      runnable_.pop();
      task(this);
    }
  }

 private:
  std::queue<runner> runnable_;
};

}  // namespace garage
