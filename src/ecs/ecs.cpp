#include "ecs/ecs.h"
#include "ecs/pool.h"
#include "ecs/entity.h"
#include "ecs/filter.h"
#include "ecs/holder.h"

namespace m2d {

namespace ecs {
  
ECS::ECS(const std::string& name, HolderWeakPtr ecs_holder)
  : name_(name)
  , ecs_holder_(ecs_holder) {}

HolderWeakPtr ECS::GetHolder() const {
  return ecs_holder_;
}

EntityWeakPtr ECS::CreateEnity(const std::vector<StringIndex>& components) {
  auto entity = pool_->AllocateEntity(entity_counter_, shared_from_this());
  ++entity_counter_;

  enities_[entity_counter_] = entity;

  entity->AddComponents(components);

  return entity;
}

PoolWeakPtr ECS::GetPool() const {
  return pool_;
}

void ECS::Tick(float delta) {
  for (auto it = enities_.begin(); it != enities_.end();) {
    if (!it->second->Tick(delta)) {
      it = enities_.erase(it);
    } else {
      ++it;
    }
  }
}

FilterWeakPtr ECS::GetOrCreateFilter(const std::vector<StringIndex>& components) {
  auto filter_bitmask = ecs_holder_.lock()->CreateComponentBitmask(components);
  auto it = filters_.find(filter_bitmask);
  if (it != filters_.end()) {
    return it->second;
  }
  
  auto filter = pool_->AllocateFilter(shared_from_this(), filter_bitmask);
  if (!filter->IsValid()) {
    return FilterWeakPtr();
  }

  filters_[filter_bitmask] = filter;
  return filter;
}

}  // namespace ecs

}  // namespace m2d
