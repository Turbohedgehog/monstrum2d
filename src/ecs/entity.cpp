#include "ecs/entity.h"

#include "ecs/ecs.h"
#include "ecs/holder.h"
#include "ecs/component_schema.h"
#include "ecs/pool.h"

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
  auto holder = ecs_.lock()->GetHolder().lock();
  auto pool = ecs_.lock()->GetPool().lock();
  for (const auto& component_name : components) {
    auto schema_id = holder->GetComponentSchemaIdByName(component_name);
    if (!schema_id) {
      continue;
    }

    auto s_id = schema_id.value();
    auto schema = holder->GetComponentSchema(s_id);
    auto component_data = schema.lock()->CreateComponentData(ecs_);
    components_[s_id] = pool->AllocateComponent(s_id, id_, component_data);
  }
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