#pragma once

#include "common/common_types.h"

namespace m2d {

namespace ecs {
  
class Component {
 public:
  Component(std::size_t type_id, std::size_t enity_id, ECSWeakPtr ecs, ComponentDataPtr data);

  ComponentDataPtr GetData() const;

 private:
  std::size_t type_id_ = 0;
  std::size_t enity_id_ = 0;
  ECSWeakPtr ecs_;
  ComponentDataPtr data_;
  
};

}  // namespace ecs

}  // namespace m2d
