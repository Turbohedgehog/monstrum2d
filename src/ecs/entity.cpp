#include "ecs/entity.h"

#include "ecs/ecs.h"
#include "ecs/holder.h"
#include "ecs/component_schema.h"
#include "ecs/pool.h"
#include "ecs/component.h"

namespace m2d {

namespace ecs {

Entity::Entity(std::size_t id, ECSWeakPtr ecs)
  : id_(id)
  , ecs_(ecs) {}

Entity::~Entity() {
  volatile int zzz = 0;
}

std::size_t Entity::GetId() const {
  return id_;
}

ECSWeakPtr Entity::GetECS() const {
  return ecs_;
}

void Entity::AddComponents(const std::vector<StringIndex>& components) {
  auto holder = ecs_.lock()->GetHolder().lock();
  auto pool = ecs_.lock()->GetPool().lock();
  for (const auto& component_index : components) {
    std::optional<std::size_t> schema_id;
    std::visit(
      visitor_overload {
        [&schema_id, &holder](const std::string& component_name) {
          schema_id = holder->GetComponentSchemaIdByName(component_name);
        },
        [&schema_id](std::size_t component_id) {
          schema_id = component_id;
        }
      },
      component_index
    );

    if (!schema_id) {
      continue;
    }

    auto s_id = schema_id.value();
    auto schema = holder->GetComponentSchema(s_id);
    auto component_data = schema.lock()->CreateComponentData(ecs_);
    components_[s_id] = pool->AllocateComponent(s_id, id_, component_data);
    component_bitmask_[s_id] = 1;
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

bool Entity::Tick(float delta) {
  for (auto it = components_.begin(); it != components_.end();) {
    if (!it->second->Tick(delta)) {
      it = components_.erase(it);
    } else {
      ++it;
    }
  }

  return !components_.empty();
}

const ComponentBitmask& Entity::GetComponentBitmask() const {
  return component_bitmask_;
}

}  // namespace ecs

}  // namespace m2d