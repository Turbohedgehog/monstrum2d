#pragma once

#include <string>
#include <map>

#include "common/common_types.h"

#include "core/core_config.h"

namespace m2d {

class Core {
 public:
  enum class Status {
    Stopped,
    Starting,
    Running,
    Stopping,
  };

  static Core& GetInstance();

  int Run(int argc, char* argv[]);
  Status GetStatus() const;
 
 private:
  Core();

  Status status_ = Status::Stopped;
  CoreConfig core_config_;

  std::size_t esc_handler_counter_ = 0;
  std::map<std::size_t, ecs::HolderPtr> esc_holders_;
};

}  // namespace m2d
