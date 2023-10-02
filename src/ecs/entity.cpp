#include "ecs/entity.h"

#include "ecs/ecs.h"
#include "ecs/holder.h"

namespace m2d {

namespace ecs {

Entity::Entity(std::size_t id, ECSWeakPtr ecs)
  : id_(id)
  , ecs_(ecs) {}

std::size_t Entity::GetId() const {
  return id_;
}

ECSWeakPtr Entity::GetECS() const {
  return ecs_;
}

void Entity::AddComponents(const std::vector<std::string>& components) {
  
}

ComponentWeakPtr Entity::GetComponent(const std::string& component_name) const {
  auto id = ecs_.lock()->GetHolder().lock()->GetComponentSchemaIdByName(component_name);
  if (!id) {
    return ComponentWeakPtr();
  }

  return GetComponentById(id.value());
}

ComponentWeakPtr Entity::GetComponentById(std::size_t component_id) const {
  auto it = components_.find(component_id);

  return it != components_.end() ? it->second : ComponentWeakPtr();
}

}  // namespace ecs

}  // namespace m2d