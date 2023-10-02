#include "ecs/ecs.h"
#include "ecs/pool.h"
#include "ecs/entity.h"

namespace m2d {

namespace ecs {
  
ECS::ECS(const std::string& name, HolderWeakPtr ecs_holder)
  : name_(name)
  , ecs_holder_(ecs_holder) {}

HolderWeakPtr ECS::GetHolder() const {
  return ecs_holder_;
}

EntityWeakPtr ECS::CreateEnity(std::vector<std::string> components) {
  auto entity = pool_->AllocateEntity(entity_counter_, shared_from_this());
  ++entity_counter_;

  enities_[entity_counter_] = entity;

  entity->AddComponents(std::move(components));

  return entity;
}

PoolWeakPtr ECS::GetPool() const {
  return pool_;
}

}  // namespace ecs

}  // namespace m2d
