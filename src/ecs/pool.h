#pragma once

#include "common/common_types.h"

namespace m2d {

namespace ecs {
  
class Pool {
 public:
  EntityPtr AllocateEntity(std::size_t id, ECSWeakPtr ecs);
};


}  // namespace ecs

}  // namespace m2d
