#include "core/core.h"

#include <chrono>
#include <iostream>
#include <filesystem>
#include <thread>

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

  InitHolders();

  status_ = Core::Status::Running;

  //std::cout << std::filesystem::current_path() << "\n";
  //std::cout << core_config_.GetName();

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

  for (const auto& system_path : core_config_.GetECSSystemDeclarationPaths()) {
    initial_holder->AppendSystems(system_path);
  }
}

void Core::InitHolders() {
  for ([[maybe_unused]] auto& [_, holder] : esc_holders_) {
    holder->Init();
  }
}

void Core::MainLoop() {
  auto fps = core_config_.GetFPS();
  auto delay = std::chrono::duration<double>(1. / fps);
  std::chrono::high_resolution_clock timer;
  auto last_update = timer.now();
  while (status_ == Status::Running && !esc_holders_.empty()) {
    std::this_thread::sleep_for(delay);
    auto current = timer.now();
    auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(current - last_update).count() / 1000000.0;
    for (auto holder_it = esc_holders_.begin(); holder_it != esc_holders_.end();) {
      auto holder = holder_it->second;
      if (holder->GetECSCount() == 0) {
        holder_it = esc_holders_.erase(holder_it);
      } else {
        holder->Update(deltaTime);
        ++holder_it;
      }
    }
  }
}

}  // namespace m2d
