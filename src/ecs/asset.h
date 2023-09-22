#pragma once

#include <string>

namespace m2d {

namespace ecs {
  
class Asset {
 public:
  virtual bool Load(const std::string& filename) = 0;
};

}  // namespace ecs

}  // namespace m2d
