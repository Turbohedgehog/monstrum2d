#include "ecs/component_schema_loader.h"

#include <stdexcept>

#include <boost/format.hpp>

#include "yaml-cpp/yaml.h"

#include "ecs/component_shema.h"

namespace m2d {

namespace ecs {

ComponentFieldPtr ParseFixedString(const YAML::Node& item) {
  return ComponentFieldPtr();
}

ComponentFieldPtr ParseInt(const YAML::Node& item) {
  return ComponentFieldPtr();
}

ComponentFieldPtr ParseDouble(const YAML::Node& item) {
  return ComponentFieldPtr();
}

std::vector<ComponentSchemaPtr> ComponentSchemaLoader::LoadComponentSchemas(
  const std::filesystem::path& schemas_path) {
  std::vector<ComponentSchemaPtr> schemas;

  auto schemas_path_str = schemas_path.generic_string();
  YAML::Node doc = YAML::LoadFile(schemas_path_str);
  if (!doc.IsSequence()) {
    throw std::invalid_argument("Wrong document format");
  }

  //auto test_schema = std::make_shared<ComponentSchema>("Test");
  //test_schema->AppendField<ComponentIntField>("id");
  //test_schema->AppendField<ComponentDoubleField>("value", 3.0);
  //schemas.emplace_back(test_schema);

  for (const auto& component_shema_node : doc) {
    auto component = component_shema_node["component"];
    if (!component) {
      continue;
    }

    auto name = component["name"];
    if (!name) {
      throw std::runtime_error((boost::format("Component without name in file '%s'") % schemas_path_str).str());
    }

    auto component_schema = std::make_shared<ComponentSchema>(name.as<std::string>());
    schemas.push_back(component_schema);

    auto schema = component["schema"];
    if (!schema) {
      continue;
    }

    if (!schema.IsSequence()) {
      throw std::runtime_error(
        (
          boost::format("[%s] Component '%s' schema item be a sequence!") %
            schemas_path_str %
            component_schema->GetName()
        ).str()
      );
    }

    for (const auto& item : schema) {
      if (!item.IsMap()) {
        throw std::runtime_error(
          (
            boost::format("[%s] Component '%s' schema must be a map!") %
              schemas_path_str %
              component_schema->GetName()
          ).str()
        );
      }

      if (item.size() == 0) {
        continue;
      }
      /*
      auto name = item["name"];
      if (!name) {
        throw std::runtime_error(
          (
            boost::format("[%s] Component '%s' schema item does not have a name!") %
              schemas_path_str %
              component_schema->GetName()
          ).str()
        );
      }
      */

      try {
        auto field_type = item.begin()->first.as<std::string>();
        if (field_type == "fixed_string") {
          component_schema->AppendField(ParseFixedString(item.begin()->second));
        } else if (field_type == "int") {
          component_schema->AppendField(ParseInt(item.begin()->second));
        } else if (field_type == "double") {
          component_schema->AppendField(ParseDouble(item.begin()->second));
        } else {
          throw std::runtime_error(
            (
              boost::format("[%s] Component '%s' shema item has unknown name '%s'!") %
                schemas_path_str %
                component_schema->GetName() %
                field_type
            ).str()
          );
        }
      } catch (const std::exception& ex) {
        throw std::runtime_error(
          (
            boost::format(
                "[%s] Error while parsing component '%s' item:\n [%s] %s") %
                schemas_path_str %
                component_schema->GetName() %
                typeid(ex).name() %
                ex.what()
          ).str()
        );
      }
    }
  }

  return schemas;
}

}  // namespace ecs

}  // namespace m2d

