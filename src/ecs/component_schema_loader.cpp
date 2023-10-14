#include "ecs/component_schema_loader.h"

#include <stdexcept>

#include <boost/format.hpp>

#include "yaml-cpp/yaml.h"

#include "ecs/component_schema.h"
#include "common/default_value.h"

namespace m2d {

namespace ecs {

ComponentFieldPtr ParseFieldNode(const std::string& field_type, const YAML::Node& item_node);

template<typename T>
ComponentFieldPtr ParsePrimitive(const YAML::Node& item) {
  auto default_value_node = item.IsScalar() ? YAML::Node() : item["default"];
  T default_value = default_value_node && !default_value_node.IsNull() ? default_value_node.as<T>() : DefaultValue<T>::value;

  return std::make_shared<ComponentPrimitiveField<T>>(default_value);
};

ComponentFieldPtr ParseArray(const YAML::Node& item) {
  auto content_node = item["content"];
  if (!content_node) {
    throw std::runtime_error("Conent node in array declaration is nit defined");
  }

  if (content_node.IsScalar()) {
    auto content_node_type = content_node.as<std::string>();
    return std::make_shared<ComponentArray>(ParseFieldNode(content_node_type, content_node));
  }

  if (content_node.IsMap()) {
    auto sub_node = content_node.begin();
    auto sub_node_type = sub_node->first.as<std::string>();
    auto sub_node_item = sub_node->second;
    return std::make_shared<ComponentArray>(ParseFieldNode(sub_node_type, sub_node_item));
  }

  throw std::runtime_error("Array content has unspecified schema");

  return ComponentFieldPtr();
}

ComponentFieldPtr ParseFieldNode(const std::string& field_type, const YAML::Node& item_node) {
  if (field_type == "int") {
    return ParsePrimitive<int>(item_node);
  }

  if (field_type == "double") {
    return ParsePrimitive<double>(item_node);
  }

  if (field_type == "string") {
    return ParsePrimitive<std::string>(item_node);
  }

  if (field_type == "array") {
    return ParseArray(item_node);
  }

  throw std::runtime_error(
    (
      boost::format("Inknown field type '%s'") %
        field_type
    ).str()
  );

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

  for (const auto& component_schema_node : doc) {
    auto component = component_schema_node["component"];
    if (!component) {
      continue;
    }

    auto name = component["name"];
    if (!name) {
      throw std::runtime_error((boost::format("Component without name in file '%s'") % schemas_path_str).str());
    }

    auto component_schema = std::make_shared<ComponentSchema>(name.as<std::string>());

    auto lifetime = component["lifetime"];
    if (lifetime) {
      if (auto ticks = lifetime["ticks"]) {
        if (!ticks.IsDefined() || !ticks.IsScalar()) {
          throw std::runtime_error(
            (
              boost::format("[%s] Component '%s' schema has wrong lifetime value!") %
                schemas_path_str %
                component_schema->GetName()
            ).str()
          );
        }

        component_schema->SetLifetime(ticks.as<int>());
      }
    }

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
        auto item_node = item.begin()->second;
        auto name = item_node["name"];
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
        
        auto name_str = name.as<std::string>();
        ComponentFieldPtr component_field_ptr = ParseFieldNode(field_type, item_node);
        if (component_field_ptr) {
          component_schema->GetRoot()->AppendField(std::move(name_str), component_field_ptr);
        } else {
          throw std::runtime_error(
            (
              boost::format("[%s] Component '%s' schema item has unknown name '%s'!") %
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

    schemas.push_back(component_schema);
  }

  return schemas;
}

}  // namespace ecs

}  // namespace m2d

