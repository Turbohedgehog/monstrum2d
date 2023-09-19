#pragma once

#include <vector>
#include <filesystem>

#include "common/common_types.h"

namespace m2d {

namespace ecs {
  
class ComponentSchemaLoader {
 public:
  static std::vector<ComponentSchemaPtr> LoadComponentSchemas(const std::filesystem::path& schemas_path);
};

}  // namespace ecs

}  // namespace m2d
