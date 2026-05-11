#include <iostream>
#include "utils/logger/logger.hpp"

int main() {
  LOG_INFO("Hello INFO");
  LOG_WARN("Hello WARN");
  LOG_ERROR("Hello ERROR");
  std::cout << "KVault Starting\n";
  return 0;
}
