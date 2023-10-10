#include "ecs/component_schema_loader.h"

#include <stdexcept>

#include <boost/format.hpp>

#include "yaml-cpp/yaml.h"

#include "ecs/component_schema.h"
#include "common/default_value.h"

namespace m2d {

namespace ecs {

template<typename T>
ComponentFieldPtr ParsePrimitive(const YAML::Node& item) {
  auto default_value_node = item["default"];
  T default_value = default_value_node ? default_value_node.as<T>() : DefaultValue<T>::value;
  return std::make_shared<ComponentPrimitiveField<T>>(default_value);
};

ComponentFieldPtr ParseArray(const YAML::Node& item) {
  static const std::map<std::string, std::function<ComponentFieldPtr()>> kFieldFactory = {
    {"int", [] { return std::make_shared<ComponentPrimitiveField<int>>(); } },
    {"double", [] { return std::make_shared<ComponentPrimitiveField<double>>(); } },
    {"string", [] { return std::make_shared<ComponentPrimitiveField<std::string>>(); } }
  };

  auto type_node = item["type"];
  if (!type_node) {
    throw std::runtime_error("type filed is not defined");
  }

  auto type_str = type_node.as<std::string>();
  auto it = kFieldFactory.find(type_str);
  if (it == kFieldFactory.end()) {
    throw std::runtime_error(
      (
        boost::format("Cannot create array of type '%s'") %
        type_str
      ).str()
    );
  }

  auto dimensions_node = item["dimensions"];
  std::size_t dimensions = dimensions_node ? dimensions_node.as<std::size_t>() : 1;

  ComponentFieldPtr content = it->second();
  
  return std::make_shared<ComponentArray>(dimensions, content);
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
    //schemas.push_back(component_schema);

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
        if (field_type == "int") {
          component_schema->GetRoot()->AppendField(std::move(name_str), ParsePrimitive<int>(item_node));
        } else if (field_type == "double") {
          component_schema->GetRoot()->AppendField(std::move(name_str), ParsePrimitive<double>(item_node));
        } else if (field_type == "string") {
          component_schema->GetRoot()->AppendField(std::move(name_str), ParsePrimitive<std::string>(item_node));
        } else if (field_type == "array") {
          try {
            component_schema->GetRoot()->AppendField(std::move(name_str), ParseArray(item_node));
          } catch (const std::exception& ex) {
            throw std::runtime_error(
              (
                boost::format("[%s] Component '%s' schema. Cannot parse array:  '%s'") %
                  schemas_path_str %
                  component_schema->GetName() %
                  ex.what()
              ).str()
            );
          }
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

