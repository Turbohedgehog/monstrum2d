#include "ecs/ecs.h"

namespace m2d {

namespace ecs {
  
ECS::ECS(ECSHolderWeakPtr ecs_holder)
  : ecs_holder_(ecs_holder) {}

}  // namespace ecs

}  // namespace m2d
