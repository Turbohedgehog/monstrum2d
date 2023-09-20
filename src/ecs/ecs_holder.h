#pragma once

#include <memory>

#include <common/common_types.h>

namespace m2d {

namespace ecs {
  
class ECSHolder : std::enable_shared_from_this<ECSHolder> {
 public:
  ECSHolder(uint32_t id);

 private:
  uint32_t id_;
};

}  // namespace ecs

}  // namespace m2d
