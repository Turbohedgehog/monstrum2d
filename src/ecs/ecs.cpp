#include "ecs/ecs.h"

namespace m2d {

ECS::ECS(ECSHandlerWeakPtr ecs_handler)
  : ecs_handler_(ecs_handler) {}

}  // namespace m2d
