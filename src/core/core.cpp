#include "core.h"

#include <iostream>

namespace m2d {

Core::Core() {}

Core& Core::GetInstance() {
  static Core core;

  return core;
}

int Core::Run(int argc, char* argv[]) {
  if (status_ != Core::Status::Stopped) {
    return 1;
  }

  core_config_.Parse(argc, argv);

  std::cout << "Monstrum2D!";

  return 0;
}

Core::Status Core::GetStatus() const {
  return status_;
}

}  // namespace m2d
