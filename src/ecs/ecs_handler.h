#pragma once

#include <memory>

#include <common/common_types.h>

namespace m2d {

class ECSHandler : std::enable_shared_from_this<ECSHandler> {
 public:
  ECSHandler(uint32_t id);

 private:
  uint32_t id_;
};

}  // namespace m2d
