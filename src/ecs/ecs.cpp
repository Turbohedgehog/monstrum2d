#include "ecs/ecs.h"
#include "ecs/pool.h"

namespace m2d {

namespace ecs {
  
ECS::ECS(HolderWeakPtr ecs_holder)
  : ecs_holder_(ecs_holder) {}

HolderWeakPtr ECS::GetHolder() const {
  return ecs_holder_;
}

EntityWeakPtr ECS::CreateEnity() {
  auto entity = pool_->AllocateEntity(entity_counter_, shared_from_this());
  ++entity_counter_;

  enities_[entity_counter_] = entity;

  return entity;
}

PoolWeakPtr ECS::GetPool() const {
  return pool_;
}

}  // namespace ecs

}  // namespace m2d