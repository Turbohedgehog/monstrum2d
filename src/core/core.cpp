#include "core.h"

#include <iostream>

namespace m2d {

Core& Core::GetInstance() {
  static Core core;

  return core;
}

int Core::Run(int *argc, char* argv[]) {
  std::cout << "Monstrum2D!";

  return 0;
}

}  // namespace m2d
