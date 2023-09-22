#pragma once

#include <memory>
#include <string>

#include <common/common_types.h>

namespace m2d {

namespace ecs {
  
class Holder : std::enable_shared_from_this<Holder> {
 public:
  Holder(uint32_t id);

 private:
  uint32_t id_;
};

}  // namespace ecs

}  // namespace m2d
