#include "ecs/component.h"

namespace m2d {

namespace ecs {

Component::Component(std::size_t type_id, std::size_t enity_id, ECSWeakPtr ecs, ComponentDataPtr data)
  : type_id_(type_id)
  , enity_id_(enity_id)
  , ecs_(ecs)
  , data_(data) {}

}  // namespace ecs

}  // namespace m2d
