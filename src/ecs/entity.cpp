#include "ecs/entity.h"

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

void Entity::AddComponents(std::initializer_list<std::string> components) {
  
}

}  // namespace ecs

}  // namespace m2d