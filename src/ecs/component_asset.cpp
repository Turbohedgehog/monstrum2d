#include "ecs/component_asset.h"

#include <fstream>
#include <stdexcept>

#include "yaml-cpp/yaml.h"

namespace m2d {

namespace ecs {

bool ComponentAsset::Load(const std::string& filename) {
  YAML::Node doc = YAML::LoadFile(filename);
  if (!doc.IsSequence()) {
    throw std::invalid_argument("Wrong document format");
  }

  return false;
}

}  // namespace ecs

}  // namespace m2d
