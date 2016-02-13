#include <iostream>
#include <stdint.h>

extern "C" {
  int fraust_init(int32_t num) {
    std::cout << num << std::endl;
  }
}
