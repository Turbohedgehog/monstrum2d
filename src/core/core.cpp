#include "core/core.h"

#include <iostream>
#include <filesystem>

#include "ecs/holder.h"

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
  try {
    core_config_.LoadConfig();
  } catch (const std::exception& ex) {
    std::cerr << "Cannot load config by reason: " << ex.what() << "\n";
    return 2;
  }

  try {
    CreateInitialHolder();
  } catch (const std::exception& ex) {
    std::cerr << "Cannot start system by reason: " << ex.what() << "\n";
    return 3;
  }

  status_ = Core::Status::Running;

  std::cout << std::filesystem::current_path() << "\n";
  std::cout << core_config_.GetName();

  MainLoop();

  status_ = Core::Status::Stopping;

  status_ = Core::Status::Stopped;

  return 0;
}

Core::Status Core::GetStatus() const {
  return status_;
}

ecs::HolderWeakPtr Core::CreatreECSHolder() {
  auto ecs_holder = std::make_shared<ecs::Holder>(esc_handler_counter_);
  esc_holders_[esc_handler_counter_] = ecs_holder;
  ++esc_handler_counter_;
  
  return ecs_holder;
}

void Core::CreateInitialHolder() {
  auto initial_holder = CreatreECSHolder().lock();
  for (const auto& schema_path : core_config_.GetECSComponentSchemaPaths()) {
    initial_holder->AppendComponentSchema(schema_path);
  }
}

void Core::MainLoop() {
  while (status_ != Status::Running) {
    for ([[maybe_unused]]  auto& [_, holder] : esc_holders_) {
      holder->Update(0.1f);
    }
  }
}

}  // namespace m2d
