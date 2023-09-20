#pragma once

#include "common/common_types.h"

namespace m2d {

namespace ecs {

class ECS {
 public:
  ECS(ECSHolderWeakPtr ecs_holder);

 private:
  ECSHolderWeakPtr ecs_holder_;
};

}  // namespace ecs

}  // namespace m2d
