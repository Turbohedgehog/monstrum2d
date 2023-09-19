#pragma once

#include <filesystem>

#include "ecs/asset.h"

namespace m2d {

namespace ecs {
  

class ComponentSchema : public Asset {
 public:
  bool Load(const std::filesystem::path& schema_path);
};

}  // namespace ecs

}  // namespace m2d
