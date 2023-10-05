#include "ecs/pool.h"

#include "ecs/entity.h"
#include "ecs/component.h"

namespace m2d {

namespace ecs {

EntityPtr Pool::AllocateEntity(std::size_t id, ECSWeakPtr ecs) {
  return std::make_shared<Entity>(id, ecs);
}

ComponentPtr Pool::AllocateComponent(
    std::size_t type_id,
    std::size_t enity_id,
    ComponentDataPtr data) {
  return std::make_shared<Component>(type_id, enity_id, data);
}

}  // namespace ecs

}  // namespace m2d


