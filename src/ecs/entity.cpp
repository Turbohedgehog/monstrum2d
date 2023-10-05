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

ComponentWeakPtr Entity::GetComponent(const StringIndex& index) const {
  std::optional<std::size_t> component_idx;
  std::visit(
    visitor_overload {
      [&component_idx](std::size_t idx) { component_idx = idx; },
      [this, &component_idx](const std::string& str_name) {
        auto id = ecs_.lock()->GetHolder().lock()->GetComponentSchemaIdByName(str_name);
        component_idx = id;
      },
    },
    index
  );

  if (!component_idx) {
    return ComponentWeakPtr();
  }

  auto it = components_.find(component_idx.value());
  if (it == components_.end()) {
    return ComponentWeakPtr();
  }

  return it->second;  
}

}  // namespace ecs

}  // namespace m2d