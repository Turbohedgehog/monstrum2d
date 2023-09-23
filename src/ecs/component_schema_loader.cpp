#include "ecs/component_schema_loader.h"

#include <stdexcept>

#include <boost/format.hpp>

#include "yaml-cpp/yaml.h"

#include "ecs/component_shema.h"

namespace m2d {

namespace ecs {

ComponentFieldPtr ParseFixedString(const std::string& name, const YAML::Node& item) {
  const static std::string kEmptyString = "";
  auto size = item["size"];
  if (!size) {
    throw std::runtime_error(
      (
        boost::format("Fixedstring '%s' does not have size field") %
          name
      ).str()
    );
  }

  auto size_value = size.as<int>();
  if (size_value <= 0) {
    throw std::runtime_error(
      (
        boost::format("Size of fixedstring '%s' must be greater than zero!") %
          name
      ).str()
    );
  }

  auto default_value_node = item["default_value"];
  auto default_value = default_value_node ? default_value_node.as<std::string>() : kEmptyString;
  return std::make_shared<ComponentFixedStringField>(name, static_cast<std::size_t>(size_value), default_value);
}

ComponentFieldPtr ParseInt(const std::string& name, const YAML::Node& item) {
  auto default_value_node = item["default_value"];
  auto default_value = default_value_node ? default_value_node.as<int>() : 0;
  return std::make_shared<ComponentIntField>(name, default_value);
}

ComponentFieldPtr ParseDouble(const std::string& name, const YAML::Node& item) {
  auto default_value_node = item["default_value"];
  auto default_value = default_value_node ? default_value_node.as<double>() : 0.;
  return std::make_shared<ComponentDoubleField>(name, default_value);
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

      try {
        auto field_type = item.begin()->first.as<std::string>();
        auto field_name = name.as<std::string>();
        auto name = item.begin()->second["name"];
        if (!name) {
          throw std::runtime_error(
            (
              boost::format("[%s] Component '%s' schema item '%s' does not have a name!") %
                schemas_path_str %
                component_schema->GetName() %
                field_type
            ).str()
          );
        }
        
        if (field_type == "fixed_string") {
          component_schema->AppendField(ParseFixedString(field_name, item.begin()->second));
        } else if (field_type == "int") {
          component_schema->AppendField(ParseInt(field_name, item.begin()->second));
        } else if (field_type == "double") {
          component_schema->AppendField(ParseDouble(field_name, item.begin()->second));
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

