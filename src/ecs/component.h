#pragma once

#include "common/common_types.h"

namespace m2d {

namespace ecs {
  
class Component {
 public:
  Component(std::size_t type_id, std::size_t enity_id, ComponentDataPtr data);

  ComponentDataPtr GetData() const;

 private:
  std::size_t type_id_ = 0;
  std::size_t enity_id_ = 0;
  ComponentDataPtr data_;
  
};

}  // namespace ecs

}  // namespace m2d
