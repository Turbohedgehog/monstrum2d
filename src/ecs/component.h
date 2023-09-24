#pragma once

#include "common/common_types.h"

namespace m2d {

namespace ecs {
  
class Component {
 public:
  Component(std::size_t type_id, std::size_t enity_id, char* data, ECSWeakPtr ecs);

 private:
  std::size_t type_id_ = 0;
  std::size_t enity_id_ = 0;
  char* data_ = nullptr;
  Component* prev_component_ = nullptr;
  Component* next_component_ = nullptr;
  
};

}  // namespace ecs

}  // namespace m2d
