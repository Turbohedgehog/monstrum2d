#include "core/core.h"

#include <iostream>
#include <filesystem>

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

  status_ = Core::Status::Starting;
  core_config_.Parse(argc, argv);

  status_ = Core::Status::Running;

  std::cout << std::filesystem::current_path() << "\n";
  std::cout << "Monstrum2D!";

  status_ = Core::Status::Stopping;

  status_ = Core::Status::Stopped;

  return 0;
}

Core::Status Core::GetStatus() const {
  return status_;
}

}  // namespace m2d
