
#include "garage/coroutine/coroutine.h"

#include <iostream>

#include "garage/channel/channel.h"

int main() {
  garage::channel<int> ch(5);
  garage::executor exec;
  exec.go([&ch](garage::executor* exec) {
    std::cout << "coroutine1 start" << std::endl;
    exec->sched([&ch](garage::executor* exec) {
      int ret = ch.recv();
      std::cout << "recv=" << ret << std::endl;
      ch << ret + 1;
      std::cout << "coroutine1 end" << std::endl;
    });
  });
  exec.go([&ch](garage::executor* exec) {
    std::cout << "coroutine2 start" << std::endl;

    ch << 1;

    exec->sched([&ch](garage::executor* exec) {
      int ret = ch.recv();
      std::cout << "recv=" << ret << std::endl;
      std::cout << "coroutine2 end" << std::endl;
    });
  });

  exec.run();
}
