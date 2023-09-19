#pragma once

#include <map>
#include <memory>
#include <string>

#include "common/common_types.h"

namespace m2d {

namespace ecs {

class ECS : public std::enable_shared_from_this<ECS> {
 public:
  ECS(HolderWeakPtr ecs_holder);
  EntityWeakPtr CreateEnity();
  HolderWeakPtr GetHolder() const;

 private:
  PoolPtr pool_;
  HolderWeakPtr ecs_holder_;
  std::size_t entity_counter_ = 0;
  std::map<std::size_t, EntityPtr> enities_;
};

}  // namespace ecs

}  // namespace m2d
