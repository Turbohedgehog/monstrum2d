#pragma once

#include <string>

#include "core_config.h"

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
};

}  // namespace m2d
