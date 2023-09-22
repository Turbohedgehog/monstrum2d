#include "ecs/component_shema.h"

#include <fstream>
#include <stdexcept>

#include "yaml-cpp/yaml.h"

namespace m2d {

namespace ecs {

bool ComponentSchema::Load(const std::filesystem::path& schema_path) {
  YAML::Node doc = YAML::LoadFile(schema_path.generic_string());
  if (!doc.IsSequence()) {
    throw std::invalid_argument("Wrong document format");
  }

  return false;
}

}  // namespace ecs

}  // namespace m2d
