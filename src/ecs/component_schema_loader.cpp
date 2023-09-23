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

  auto test_schema = std::make_shared<ComponentSchema>("Test");
  test_schema->AppendField<ComponentIntField>("id");
  test_schema->AppendField<ComponentDoubleField>("value", 3.0);
  schemas.emplace_back(test_schema);

  for (auto component_shema_node : doc) {
    //component_shema_node.
  }

  return schemas;
}

}  // namespace ecs

}  // namespace m2d

