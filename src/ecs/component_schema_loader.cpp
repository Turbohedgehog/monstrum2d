#include "ecs/component_schema_loader.h"

#include "yaml-cpp/yaml.h"

#include "ecs/component_shema.h"

namespace m2d {

namespace ecs {

std::vector<ComponentSchemaPtr> ComponentSchemaLoader::LoadComponentSchemas(
  const std::filesystem::path& schemas_path) {
  std::vector<ComponentSchemaPtr> schemas;

  YAML::Node doc = YAML::LoadFile(schemas_path.generic_string());
  if (!doc.IsSequence()) {
    throw std::invalid_argument("Wrong document format");
  }

  for (auto component_shema_node : doc) {
    //component_shema_node.
  }

  return schemas;
}

}  // namespace ecs

}  // namespace m2d

