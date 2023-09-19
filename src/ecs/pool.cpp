#include "ecs/pool.h"

#include "ecs/entity.h"

namespace m2d {

namespace ecs {

EntityPtr Pool::AllocateEntity(std::size_t id, ECSWeakPtr ecs) {
  return std::make_shared<Entity>(id, ecs);
}

}  // namespace ecs

}  // namespace m2d


