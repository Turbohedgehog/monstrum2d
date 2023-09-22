#include "ecs/ecs.h"
#include "ecs/pool.h"

namespace m2d {

namespace ecs {
  
ECS::ECS(HolderWeakPtr ecs_holder)
  : ecs_holder_(ecs_holder) {}

EntityWeakPtr ECS::CreateEnity() {
  auto entity = pool_->AllocateEntity(entity_counter_, shared_from_this());
  ++entity_counter_;

  enities_[entity_counter_] = entity;

  return entity;
}

}  // namespace ecs

}  // namespace m2d
