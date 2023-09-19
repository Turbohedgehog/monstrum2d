#pragma once

#include <string>

#include "ecs/component_data.h"

namespace m2d {

namespace ecs {
  
class Component {
 public:
  
 private:
  std::string name_;
  ComponentData data_;
};

}  // namespace ecs

}  // namespace m2d
