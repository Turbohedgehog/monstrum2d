#pragma once

#include "common/common_types.h"

namespace m2d {

namespace ecs {
  
class Pool {
 public:
  EntityPtr AllocateEntity(std::size_t id, ECSWeakPtr ecs);
  ComponentPtr AllocateComponent(std::size_t type_id, std::size_t enity_id, ComponentSchemaWeakPtr schema, ComponentDataPtr data);
  FilterPtr AllocateFilter(ECSWeakPtr ecs, const ComponentBitmask& component_bitmask);
};


}  // namespace ecs

}  // namespace m2d
