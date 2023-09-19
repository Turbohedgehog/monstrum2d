#pragma once

#include "common/common_types.h"

namespace m2d {

class ECS {
 public:
  ECS(ECSHandlerWeakPtr ecs_handler);

 private:
  ECSHandlerWeakPtr ecs_handler_;
};

}  // namespace m2d
